/*
 * @Author: your name
 * @Date: 2021-02-23 14:25:09
 * @LastEditTime: 2021-02-23 14:45:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_11_realsea.cpp
 */
//
//  5.11.cpp 同步操作和强制排序->释放队列与同步
//  Cpp-Concurrency
//  使用原子操作从队列中读取数据

/*
一种处理方式是让线程产生数据，并存储到一个共享缓存中，而后调用count.store(number_of_items,memory_order_release)①让其他线程知道数据是可用的.
线程群会消耗队列中的元素，之后可能调用count.fetch_sub(1, memory_order_acquire)②向队列索取一个元素。在这之前，需要对共享缓存进行完整的读取④。
一旦count归零，那么队列中就没有元素了，当没有元素耗线程必须等待③


    当只有一个消费者线程时还好，fetch_sub()是一个带有memory_order_acquire的读取操作，并且存储操作是带有
memory_order_release语义，所以存储与加载同步，线程可以从缓存中读取元素。当有两个读取线程时，第二个
fetch_sub()操作将看到被第一个线程修改的值，且没有值通过store写入其中。先不管释放序列的规则，第二个线程与第
一个线程不存在先行关系，并且对共享缓存中值的读取也不安全，除非第一个fetch_sub()是带有memory_order_release
语义的，这个语义为两个消费者线程建立了不必要的同步。无论是释放序列的规则，还是带有memory_order_release语
义的fetch_sub操作，第二个消费者看到的是一个空的queue_data，无法从其获取任何数据，并且还会产生条件竞争。幸
运的是，第一个fetch_sub()对释放顺序做了一些事情，所以store()能同步与第二个fetch_sub()操作。两个消费者线程间
不需要同步关系。这个过程在图5.7中展示，其中虚线表示的就是释放顺序，实线表示的是先行关系。

*/


#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
std::vector<int> queue_data;
std::atomic<int> count;

// 占据队列
void populate_queue() {
    unsigned const number_of_items = 20;
    queue_data.clear();
    for (unsigned i = 0; i < number_of_items; ++i) {
        queue_data.push_back(i);
    }
    count.store(number_of_items, std::memory_order_release); //1 初始化储存
}

void wait_for_more_items()
{
    std::this_thread::sleep_for(std::chrono::microseconds(1));
}

void process( int i )
{
    cout<<"i= "<<i<<endl;
}
// 读取队列
void consume_queue_items() {
    while (true) {
        int item_index;
        if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) { // 2 一个“读-改-写”操作
            wait_for_more_items(); //3 等待更多元素
            continue;
        }
        cout<<"item_index = "<<item_index<<endl;
        process(queue_data[item_index - 1]); //4 安全读取queue_data
    }
}

int main() {
    std::thread a(populate_queue);
    std::thread b(consume_queue_items);
    std::thread c(consume_queue_items);
    
    a.join();
    b.join();
    c.join();
}
