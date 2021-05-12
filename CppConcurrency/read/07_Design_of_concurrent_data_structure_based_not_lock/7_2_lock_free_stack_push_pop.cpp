/*
 * @Author: your name
 * @Date: 2021-02-25 17:15:48
 * @LastEditTime: 2021-02-25 17:59:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/07_Design_of_concurrent_data_structure_based_not_lock/7_2_lock_free_stack_push_pop.cpp
 */
//
//  7.2.cpp 无锁数据结构的例子->写一个无锁的线程安全栈
//  Cpp-Concurrency
//
//  删除一个节点
//  1.读取当前head指针的值
//  2.读取head->next
//  3.设置head到head->next
//  4.通过索引node,返回data数据
//  5.删除索引节点
//  多线程情况->读取到了同一个head->一个到步骤5，另一个到步骤2，步骤2的线程会解引用一个空指针
//  所以我们现在只能跳过节点5，让节点泄漏
//  多线程情况->两个线程读取到同一个head，返回同一个节点
//  带有节点泄漏的无锁栈

#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <chrono>

using namespace std;

template <typename T>
class lock_free_stack {
private:
    struct node {
        std::shared_ptr<T> data;
        node* next;
        //node(T const & data_): data(std::shared_ptr<T>(data_)) {}
        //注意make_shared比直接shared_ptr构造的内存开销小
        node(T const & data_): data(std::make_shared<T>(data_)) {}
        //node(T const & data_): data(new T(data_)) {}
        
    };
    std::atomic<node*> head;
public:
    void push(T const& data) {
        node* const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::shared_ptr<T> pop() {
        node* old_head = head.load();
        while (old_head && // 在解引用前检查是否为空指针
               !head.compare_exchange_weak(old_head, old_head->next));
        return old_head ? old_head->data : std::shared_ptr<T>();
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
    std::thread th2(popthread); 
    th1.join();
    th2.join();
    return 0;
}
