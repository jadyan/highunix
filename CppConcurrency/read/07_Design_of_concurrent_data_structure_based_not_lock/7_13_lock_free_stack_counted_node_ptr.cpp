/*
 * @Author: your name
 * @Date: 2021-03-01 17:26:11
 * @LastEditTime: 2021-03-02 11:02:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/07_Design_of_concurrent_data_structure_based_not_lock/7_13_lock_free_stack_counted_node_ptr.cpp
 https://blog.csdn.net/weixin_43376501/article/details/108325765
 */
//基于引用计数和松散原子操作的无锁栈
//通过使用更多的松散内存序操作，不影响并发性的同时提高性能
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>

using namespace std;

template <typename T>
class lock_free_stack
{
private:
    struct node;
    struct counted_node_ptr
    {
        int external_count;
        node *ptr;
    };

    struct node
    {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        counted_node_ptr next;

        node(T const &data_) : data(std::make_shared<T>(data_)),
                               internal_count(0)
        {
        }
    };

    std::atomic<counted_node_ptr> head;

    void increase_head_count(counted_node_ptr &old_counter)
    {
        counted_node_ptr new_counter;

        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!head.compare_exchange_strong(old_counter, new_counter,
                                               std::memory_order_acquire,
                                               std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }

public:
    ~lock_free_stack()
    {
        while (pop())
            ;
    }

    void push(T const &data)
    {
        counted_node_ptr new_node;
        new_node.ptr = new node(data);
        new_node.external_count = 1;
        new_node.ptr->next = head.load(std::memory_order_relaxed);
        while (!head.compare_exchange_weak(new_node.ptr->next, new_node,
                                           std::memory_order_release,
                                           std::memory_order_relaxed))
            ;
    }
    std::shared_ptr<T> pop()
    {
        counted_node_ptr old_head =
            head.load(std::memory_order_relaxed);
        for (;;)
        {
            increase_head_count(old_head);
            node *const ptr = old_head.ptr;
            if (!ptr)
            {
                return std::shared_ptr<T>();
            }
            if (head.compare_exchange_strong(old_head, ptr->next,
                                             std::memory_order_relaxed))
            {
                std::shared_ptr<T> res;
                res.swap(ptr->data);

                int const count_increase = old_head.external_count - 2;

                if (ptr->internal_count.fetch_add(count_increase,
                                                  std::memory_order_release) == -count_increase)
                {
                    delete ptr;
                }

                return res;
            }
            else if (ptr->internal_count.fetch_add(-1,
                                                   std::memory_order_relaxed) == 1)
            {
                ptr->internal_count.load(std::memory_order_acquire);
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


void pushthread()
{

    for(int i=0;i<100;i++)
    {
        cout<<"push stack"<<i<<endl;
        lock_free_stack<int>::instance().push(i);
        // lock_free_stack<int> lfs;
        // lfs.push(i);
    }

}

void popthread()
{
    while (true)
    {
        std::shared_ptr<int> pi = lock_free_stack<int>::instance().pop();
        if(pi !=nullptr)
        {
            cout<<"pop stack"<<*pi<<endl;   
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
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    return 0;
}