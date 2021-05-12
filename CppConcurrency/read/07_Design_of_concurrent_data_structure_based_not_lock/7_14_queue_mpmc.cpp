#include <iostream>
#include <functional>
#include <deque>
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>

//A lock-free queue with helping

/*
https://github.com/kicsy/flying/blob/2a575b53630d846bc1169420966031144de8a101/FreeQueue.h
就需要两个node指针：head和tail。可以让多线程对它们进行访问，所以这两个节点最好是原子的，这样就不用考虑互斥问题了。


tail           head
[]<--[]<--[]<--[]
使用计数方式


data指针原子化，并通过“比较/交换”操作对其进行设置。
如果“比较/交换”成功，
就说明能获取tail指针，
并能够安全的对其next指针进行设置，也就是更新tail。因为有其他线程对数据进行了存储，
所以会导致“比较/交换”操作的失败，这时就要重新读取tail，重新循环。
当原子操作对于std::shared_ptr<>是无锁的，就可以轻松一下了。如果不是，就需要一个替代方案；
一种可能是让pop()函数返回一个std::unique_ptr<>(毕竟，这个指针只能引用指定对象)，
并且将数据作为普通指针存储在队列中的方案。这就需要队列支持存储std::atomic<T*>类型，
对于compare_exchange_strong()的调用就很有必要了。来解决多线程对pop()和push()的访问。
*/

template<typename T>
class lock_free_queue
{
private:
	struct node;
	struct counted_node_ptr
	{
		int external_count;
		node* ptr;
	};

    //这里需要2bit的external_counters，因为最多就有两个计数器。因为使用了位域，
    //所以就将internal_count指定为30bit的值，就能保证计数器的总体大小是32bit。
    //内部计数值就有充足的空间来保证这个结构体能放在一个机器字中(包括32位和64位平台)。
    //为的就是避免条件竞争，将结构体作为单独的实体来更新。
    //让结构体的大小保持在一个机器字内，对其的操作就如同原子操作一样，还可以在多个平台上使用。
	struct node_counter
	{
		int internal_count:30;           //30bit 保证计数器总体大小是32bit 内部计数器值就有足够空间来保证这个结构体能放在一个机器字中
		unsigned external_counters:2;    // 2bit 因为最多就两个计数器
	};
	struct node
	{
		std::atomic<T*> data;
		std::atomic<node_counter> count;    // internal_count 被替换成 count,包含两个数据成员
		std::atomic<counted_node_ptr> next; // #1
		node()
		{
			node_counter new_count;
			new_count.internal_count=0;
			new_count.external_counters=2;  // 因为当新节点加入队列时，都会被tail和上一个节点的next指针所指向
			count.store(new_count);
			counted_node_ptr next_node={0};
			next.store(next_node);
		}
		void release_ref()
		{
			node_counter old_counter= count.load(std::memory_order_relaxed);
			node_counter new_counter;
			do
			{
				new_counter=old_counter;
				--new_counter.internal_count;
			}
			while(!count.compare_exchange_strong(
				old_counter,new_counter,
				std::memory_order_acquire,std::memory_order_relaxed));
			if(!new_counter.internal_count &&
				!new_counter.external_counters)
			{
				delete this;
			}
		}
	};
	std::atomic<counted_node_ptr> head;
	std::atomic<counted_node_ptr> tail;
	static void increase_external_count(std::atomic<counted_node_ptr>& counter, counted_node_ptr& old_counter)
	{
		counted_node_ptr new_counter;
		do
		{
			new_counter=old_counter;
			++new_counter.external_count;
		}
		while(!counter.compare_exchange_strong(
			old_counter,new_counter,
			std::memory_order_acquire,
			std::memory_order_relaxed));
		old_counter.external_count=new_counter.external_count;
	}
    //更新tail指针
	void set_new_tail(counted_node_ptr &old_tail,// #6
		counted_node_ptr const &new_tail)
	{

		node* const current_tail_ptr=old_tail.ptr;

        // 使用compare_exchange_weak更新tail
		while(!tail.compare_exchange_weak(old_tail,new_tail) &&// #7
			old_tail.ptr==current_tail_ptr);
        
        // 当“比较/交换”失败时，先判断新旧ptr是否一样
		if(old_tail.ptr==current_tail_ptr)// #8
		{
            //相同时，表示对tail的设置已经完成，释放旧外部计数器
			free_external_counter(old_tail);// #9
		}
		else
		{
            //不同时，另一线程已经释放了ptr，只需要将这次引用进行释放即可
			current_tail_ptr->release_ref();// #10
		}
	}
	static void free_external_counter(counted_node_ptr &old_node_ptr)
	{
		node* const ptr=old_node_ptr.ptr;
		int const count_increase=old_node_ptr.external_count-2;
		node_counter old_counter=
			ptr->count.load(std::memory_order_relaxed);
		node_counter new_counter;
		do
		{
			new_counter=old_counter;
			--new_counter.external_counters;
			new_counter.internal_count+=count_increase;
		}
		while(!ptr->count.compare_exchange_strong(
			old_counter,new_counter,
			std::memory_order_acquire,std::memory_order_relaxed));
		if(!new_counter.internal_count &&
			!new_counter.external_counters)
		{
			delete ptr;
		}
	}
public:
	lock_free_queue()
	{
		counted_node_ptr new_node;
		new_node.external_count=1;
		new_node.ptr=new node;
		head.store(new_node);
		tail.store(new_node);
	}
	lock_free_queue(const lock_free_queue& other)=delete;
	lock_free_queue& operator=(const lock_free_queue& other)=delete;
	~lock_free_queue()
	{
		while(pop());
		delete head.load().ptr;
	}
	std::unique_ptr<T> pop()
	{
		counted_node_ptr old_head=head.load(std::memory_order_relaxed);
		for(;;)
		{
            //increase_external_count()增加计数器的计数 外部引用计数加1
			increase_external_count(head,old_head);
			node* const ptr=old_head.ptr;
			if(ptr==tail.load().ptr)
			{
				return std::unique_ptr<T>();
			}
			counted_node_ptr next=ptr->next.load();// #2
			if(head.compare_exchange_strong(old_head,next))
			{
				T* const res=ptr->data.exchange(NULL);
				free_external_counter(old_head);
				return std::unique_ptr<T>(res);
			}
			ptr->release_ref();
		}
	}
	void push(T new_value)
	{
		std::unique_ptr<T> new_data(new T(new_value));
		counted_node_ptr new_next;
		new_next.ptr=new node;
		new_next.external_count=1;
		counted_node_ptr old_tail=tail.load();
		for(;;)
		{
            //第一次进来就是NUll
            //increase_external_count()增加计数器的计数 外部引用计数加1
			increase_external_count(tail,old_tail);
			T* old_data=NULL;
            // 对节点的data设置时
			if(old_tail.ptr->data.compare_exchange_strong(old_data,new_data.get()))
			{
				counted_node_ptr old_next={0};
                // 使用 compare_exchange_strong()来避免循环
				if(!old_tail.ptr->next.compare_exchange_strong(// #3
					old_next,new_next))
				{
					delete new_next.ptr; // 当交换失败，删除一开始分配的节点
					new_next=old_next;   // 并且重新分配节点
				}
				set_new_tail(old_tail, new_next);
				new_data.release();
				break;
			}
            // 未能在循环阶段对data指针进行设置时，帮助成功的线程完成更新
			else
			{
				counted_node_ptr old_next={0};
                // 尝试更新next指针，让其指向该线程分配的新节点
				if(old_tail.ptr->next.compare_exchange_strong(// #11
					old_next,new_next))
				{
					old_next=new_next;        //新节点作为新的tail节点
					new_next.ptr=new node;    // 分配另一个新节点，用来管理队列中新推送的数据项
				}
				set_new_tail(old_tail, old_next);    // 设置tail节点
			}
		}
	}
};


lock_free_queue<int> q;

void pushthread()
{
    for (int i = 1; i < 1000; ++i) {
        printf("pushing %d\n", i);
        q.push(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void popthread()
{
    int i = 0;
    while (1) {
        std::shared_ptr<int> p = q.pop();
        if (p) {
            printf("poping %d\n", *p);
            ++i;
        }
    }
}

int main()
{
    
    std::thread th1(pushthread); 
    std::thread th3(pushthread); 
    std::thread th4(pushthread); 
    std::thread th9(pushthread);
    std::thread th10(pushthread);


    std::thread th2(popthread); 
    std::thread th5(popthread);
    std::thread th6(popthread);
    std::thread th7(popthread);
    std::thread th8(popthread);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    return 0;
}
