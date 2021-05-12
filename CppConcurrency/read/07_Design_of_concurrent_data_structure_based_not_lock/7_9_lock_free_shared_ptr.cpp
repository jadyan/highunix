//
//  7.9.cpp 无锁数据结构的例子->检测使用引用计数的节点
//  Cpp-Concurrency
//

/*
双引用计数法需要一个外部计数器(external counter)和一个内部计数器(internal counter)，这两个计数器的和就是引用当前这个节点的
个数。外部计数器与指向节点的指针绑定在一起，每当读取一次节点的指针的时候，外部计数器就加1。当读取完成后，内部计数器就会减去1。
也就是说，读取这个节点的指针的时候，外部计数器加1，读取完成后，内部计数器减1。当某个节点从stack中取出来后(此时这
个节点在其他线程中可能被引用了，也可能没被引用)，此节点的内部计数器就要加上此节点的外部计数器的值并减去
1(internal_counter = external_counter - 1)。一旦内部计数器的值为0，就表示没有其他线程引用这个节点了，只有当前线程引用这个
节点，因此当前线程就可以安全的回收这个节点的内存了。此外，一旦这个节点已经从stack中摘除，那么这个节点的外部计数器就不需要了，此时只需要内部计数器就行了。内部计数器用于当前线程或者其他线程来判断是否可以安全回收这个节点内存。我们来看一下最终的实现(这里书上给的是优化后的实现，其实比较难以理解，得多看看，注意多看代码里的注释)：

*/

#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <chrono>

template <typename T>
class lock_free_stack {
private:
    struct node;
    struct counted_node_ptr {
        // 注意，外部计数器用的是普通变量，因为外部计数器(或者说这个counted_node_ptr结构体对象)
		// 一般都是拷贝副本，并不会对单一对象执行并发访问
        int external_count; // 外部引用计数
        node* ptr;
    };
    struct node {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count; //  内部计数器使用原子变量，因为内部计数器在节点内部，且这个点会被new出来，且被多个线程共享
        counted_node_ptr next;   // 注意，这里next并不是指针，而是一个结构体，一个包含外部计数器和下一个node指针的结构体，并不是指针
        node(T const& data_) : data(std::make_shared<T>(data_)), internal_count(0) {}
    };

    // 特化std::atomic<> 模板来对head进行声明，counted_node_ptr体积够小，能让std::atomic<>无锁->因为平台支持”双字比较和交换“，如果平台不支持，最好还是使用std::shared_ptr
    std::atomic<counted_node_ptr> head; 
    
    // 进入pop函数的第一件事就是将head的外部计数器递增，表示当前有一个线程打算读取node指针
    void increase_head_count(counted_node_ptr& old_counter) {
        counted_node_ptr new_counter;
        do {
            // 此循环是为了能够确保head能被此线程获取并将其外部节点递增
            new_counter = old_counter;
            ++new_counter.external_count;
        }
        while (!head.compare_exchange_strong(old_counter, new_counter)); // 注意，这里不再是对比指针，而是通过二进制方式对比结构体对象是否相等
        //// 此处有可能发生线程切换，导致old_counter与当前真正的head不一致，不过没事，因为已经将之前的head的外部节点递增了，另一个线程可以负责处理那个head
        old_counter.external_count = new_counter.external_count;
    }
public:
    ~lock_free_stack() {
        while (pop());
    }
    
    void push(T const& data) {
        counted_node_ptr new_node;         // 这里并没有new出一个，直接创建一个栈对象
        new_node.ptr = new node(data);     // 真正的节点内容
        new_node.external_count = 1;       // 新节点，只有一个head指针的外部引用
        new_node.ptr->next = head.load();  // 将外部节点初始化为1，因为当前有head在引用
        while (!head.compare_exchange_weak(new_node.ptr->next, new_node));
    }
    
    // pop()
    std::shared_ptr<T> pop() {
        //先尝试获取当前head，不过，最终获取的以increase_head_count返回的为主
        counted_node_ptr old_head = head.load();
        for (; ;) {
            // 真正获取当前head，并将head的外部计数器递增1。注意，此时等此函数退出时，old_head并不一定等于当前stack的head
            increase_head_count(old_head); //外部引用计数增加后，才能访问指向的节点

            //假如不在获取指针之前对外部计数器递增，则其他线程可能会释放这个ptr，导致当前线程的ptr变成了悬垂指针
            node* const ptr = old_head.ptr;
            if (!ptr) {
                return std::shared_ptr<T>();
            }

            // 这里的if才是真正尝试将head从stack中移除，如果发现当前的head与刚才获取的old_head不一致，说明当前线程在
            // 获取old_head并将head的外部计数器递增1后，另一个线程将这个递增后的head给移除了，并没有轮到当前线程来处理
            if (head.compare_exchange_strong(old_head, ptr->next)) { // 当指针不为空
                std::shared_ptr<T> res;
                res.swap(ptr->data);
                // 如果其他线程并没有引用old_head中的node指针，则理论上old_head中的外部计数器的值是2，因为刚才在increase_head_count
				// 中对其进行了递增。如果此时其他线程也引用了old_head中对应的node指针，则此时old_head中的外部计数器的值一定大于2，且
				// 减去2之后的值就是其他线程引用的个数(或者你可以这么理解，由于当前线程将不再引用这个节点，因此要把外部计数器减去1。然后
				// 由于这个head节点已经从stack中移除了，所以stack不再引用这个节点了，因此外部引用计数又再次减去1)。然后就需要比较外部
				// 计数器与内部计数器之和是否为0。如果之和为0，则表示现在没有其他线程引用这个节点了，那么就可以安全的删除这个节点了。
				// 注意，如果它们之和为0，则表示internal_count之前的数值一定是-count_increase的值，因为fetch_add返回的是旧值，所以你
				// 会发现下面比较有点怪异，不是比较0，而是比较旧值与-count_increase。
                int const count_increase = old_head.external_count - 2; // 相加的值要-2
                if (ptr->internal_count.fetch_add(count_increase) == -count_increase) { // 使用原子操作fetch_add，将外部计数加到内部计数中去->如果现在引用计数为0，那么之前fetch_add返回的值在相加之前一定是一个负数，这种情况下可以删除节点
                    delete ptr;
                }
                return res; // 无论节点是否删除，都返回获取的数据
            }
            else if (ptr && ptr->internal_count.fetch_sub(1) == 1) { // 当比较/交换 失败，说明节点在之前被删除或者添加了一个新节点到栈中->所以要将当前节点上的引用计数-1，因为当前线程已经无法访问这个节点了，如果当前线程是最后一个持有引用，那么内部引用计数将会为1，即可以删除该节点
                // 不管是由于其他线程已经把这个节点的内容返回了，还是其他线程新加了节点到stack中，此时都要重新循环，从而重新得到新的head并pop。
				// 但是在重新循环之前，由于上面在获取head时已经将head外部计数器加1了，那么这里需要将内部计数器减去1，表示当前线程不再引用这个节
				// 点了。如果发现内部计数器减去1之后变成了0，则表示内部计数器之前是1，所以肯定有其他线程已经返回这个节点的内容了(只要确定内部
				// 计数器的值大于0，就表示肯定有其他线程已经进入了上面的if分支并且会把节点中的值返回)，且如果正巧发现内部计数器的值为1，则表示
				// 当前已经没有其他线程再引用这个节点了(因为当前线程马上就要将内部计数器减1，则内部计数器就变成了0，就表示没有任何线程拥有这个
				// 节点了)。因此，此时就可以直接删除这个节点了
                delete ptr;
            }
        }
    }
    static lock_free_stack& instance()
    {
        static lock_free_stack lfs;
        return lfs;
    }
};


lock_free_stack<int>  s;

//void push(lock_free_stack<int>* s)
void push()
{
    for (int i = 0; i < 10; ++i) {
        printf("pushing %d\n", i);
        s.push(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

//void pop(lock_free_stack<int>* s)
void pop()
{
    int count = 0;
    std::shared_ptr<int> e;
    while (true) {
        if (e = s.pop()) {
        
            printf("thread id %d popping %d\n", std::this_thread::get_id(), *e);
            ++count;
        }
        // }else{
        //     printf("thread id %d popping NULL\n", std::this_thread::get_id());
        // }
    }
}

int main()
{
    std::shared_ptr<int> sp = std::make_shared<int>(1);
    //printf("std::atomic_is_lock_free(std::shared_ptr): %d\n", 
    //        std::atomic_is_lock_free(&sp));

    //指针不能这样传值 必须std::move, 不然不是同一个
    //lock_free_stack<int> s;
    //std::thread t1(push, &s);
    //std::thread t2(pop, &s);

    std::thread t1(push);
    std::thread t2(pop);


    // std::thread t11(push);
    // std::thread t12(push);
    // std::thread t13(push);

    std::thread t21(pop);
    std::thread t22(pop);
    std::thread t23(pop);
    t1.join();
    t2.join();


    t21.join();
    t22.join();
    t23.join();
    return 0;
}