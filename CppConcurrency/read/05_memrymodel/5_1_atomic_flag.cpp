/*
 * @Author: your name
 * @Date: 2021-02-08 14:59:58
 * @LastEditTime: 2021-02-22 14:43:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5.1.cpp
 */
//
//  5.1.cpp C++中的原子操作和原子类型->std::atomic_flag的相关操作
//  Cpp-Concurrency
//
//  使用 std::atomic_flag 实现自旋互斥锁
//  最最基本的互斥量，但是已经足够std::mutex_guard 使用了
//  但是 std::atomic_flag 没有非修改查询操作，也不能像普通的布尔标志那样使用
//  所以最好使用std::atomic<bool>
//  虽然他依然不能拷贝构造和拷贝赋值，但是你可以使用一个非原子类型的bool类型构造它

/*
自旋锁是一种非阻塞锁，也就是说，如果某线程需要获取自旋锁，但该锁已经被其他线程占用时，该线程不会被挂起，
而是在不断的消耗CPU的时间，
不停的试图获取自旋锁。

互斥量是阻塞锁，当某线程无法获取互斥量时，该线程会被直接挂起，该线程不再消耗CPU时间，
当其他线程释放互斥量后，操作系统会激活那个被挂起的线程，让其投入运行。

不能拷贝构造另一个std::atomic_flag对象；并且，不能将一个对象赋予另一个std::atomic_flag对象。这不是std::atomic_flag特有的，而是所有原子类型共有的。
*/

#include <iostream>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
class spinlock_mutex {
    std::atomic_flag flag;
public:
    spinlock_mutex(): flag(ATOMIC_FLAG_INIT) {}
    void lock() {
        //如果之前 std::atomic_flag 对象已被设置，则返回 true，否则返回 false
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
    static spinlock_mutex& singlespinmutex()
    {
        static spinlock_mutex sm;
        return sm;
    }
};


//spinlock_mutex sm;

void run()
{
    
    int i=0;
    while (true)
    {
        spinlock_mutex::singlespinmutex().lock();
        cout<<"线程[ "<<std::this_thread::get_id()<<" ]执行 i= "<<i++<<endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        spinlock_mutex::singlespinmutex().unlock();
    }
    
}

int main(int argc , char ** argv)
{
    std::vector<std::thread> threads;

    for (int i = 1; i <= 10; ++i)
    {
        threads.push_back(std::thread(run));
    }

    for (auto & th:threads)
        th.join();
}
