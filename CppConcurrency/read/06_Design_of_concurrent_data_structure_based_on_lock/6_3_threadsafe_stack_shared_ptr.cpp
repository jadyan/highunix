/*
 * @Author: your name
 * @Date: 2021-02-24 09:32:26
 * @LastEditTime: 2021-02-26 17:12:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/06_Design_of_concurrent_data_structure_based_on_lock/6_3_threadsafe_stack_shared_ptr.cpp
 */
//
//  6.3.cpp 基于锁的并发数据结构->线程安全队列一一使用锁和条件变量
//  Cpp-Concurrency
//
//  使用 std::shared_ptr<> 实例的线程安全队列
//  优点：
//  新的实例分配结束时，不会被锁在push()中
//  而且实例分配相比内存分配，性能有了很大的提升
//  总结->
//  提升性能，减少了互斥量持有的时间，允许其他线程在分配内存的同时，对队列进行其他的操作




/*
queue 和 stack 有一些成员函数相似，但在一些情况下，工作方式有些不同：
    front()：返回 queue 中第一个元素的引用。如果 queue 是常量，就返回一个常引用；如果 queue 为空，返回值是未定义的。
    back()：返回 queue 中最后一个元素的引用。如果 queue 是常量，就返回一个常引用；如果 queue 为空，返回值是未定义的。
    push(const T& obj)：在 queue 的尾部添加一个元素的副本。这是通过调用底层容器的成员函数 push_back() 来完成的。
    push(T&& obj)：以移动的方式在 queue 的尾部添加元素。这是通过调用底层容器的具有右值引用参数的成员函数 push_back() 来完成的。
    pop()：删除 queue 中的第一个元素。
    size()：返回 queue 中元素的个数。
    empty()：如果 queue 中没有元素的话，返回 true。
    emplace()：用传给 emplace() 的参数调用 T 的构造函数，在 queue 的尾部生成对象。
    swap(queue<T> &other_q)：将当前 queue 中的元素和参数 queue 中的元素交换。它们需要包含相同类型的元素。也可以调用全局函数模板 swap() 来完成同样的操作。

*/

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <chrono>

using  namespace std;

template <typename T>
class threadsafe_queue {
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T> > data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue() {}
    
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(*data_queue.front());  //弹出函数会持有一个变量的引用，为了接收这个新值，必须对存储的指针进行解引用
        data_queue.pop();
    }
    
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(*data_queue.front()); //弹出函数会持有一个变量的引用，为了接收这个新值，必须对存储的指针进行解引用 
        data_queue.pop();
    }
    
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }
    
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
    
    void push(T new_value) {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        //std::shared_ptr<T> data(std::make_shared<T>(new_value));
        //std::shared_ptr<T> data(new T(new_value));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
};



#if 0
int main() {
    threadsafe_queue<int> si;
    si.push(5);
    si.push(6);
    std::shared_ptr<int> p = si.wait_and_pop();
    std::cout<<"1.p= "<<*p<<std::endl;
    if (!si.empty()) {
        int x;
        si.wait_and_pop(x);
        std::cout<<"x= "<<x<<std::endl;
    }
}
#endif



threadsafe_queue<int> si;

void pushthread()
{
    for (int i=1;i<100;i++){
        si.push(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void popthread()
{
    while(true)
    {
        std::shared_ptr<int> p = si.wait_and_pop();
        std::cout<<"1.p= "<<*p<<std::endl;
        if (!si.empty()) {
            int x;
            si.wait_and_pop(x);
            std::cout<<"x= "<<x<<std::endl;
        }
    }
}

int main()
{
    std::thread th1(pushthread);
    std::thread th2(pushthread);
    std::thread th3(pushthread);
    std::thread th4(pushthread);
    std::thread th5(pushthread);



    std::thread th6(popthread);
    std::thread th7(popthread);
    std::thread th8(popthread);
    std::thread th9(popthread);
    std::thread th10(popthread);

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

}