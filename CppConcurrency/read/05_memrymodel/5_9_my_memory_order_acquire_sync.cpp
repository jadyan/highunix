/*
 * @Author: your name
 * @Date: 2021-02-23 10:36:55
 * @LastEditTime: 2021-02-23 11:16:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_9_memory_order_acquire_sync.cpp
 */
//
//  5.9.cpp 同步操作和强制排序->原子操作中的内存顺序
//  获取-释放序列
//  Cpp-Concurrency
//
//  Copyright © 2017年 kelele67. All rights reserved.
//  使用获取和释放顺序进行同步传递

/*
尽管thread_2只接触到变量syn1②和sync2③，不过这对于thread_1和thread_3的同步就足够了，这能保证断言不会触发。
首先，thread_1将数据存储到data中先行于存储sync1①(它们在同一个线程内)。
因为加载sync1①的是一个while循环，它最终会看到thread_1存储的值(是从“释放-获取”对的后半对获取)。
因此，对于sync1的存储先行于最终对于sync1的加载(在while循环中)。thread_3的加载操作④，
位于存储sync2③操作的前面(也就是先行)。存储sync2③因此先行于thread_3的加载④，加载又先行于存储sync2③，
存储sync2又先行于加载sync2④，加载syn2又先行于加载data。
因此，thread_1存储数据到data的操作先行于thread_3中对data的加载，并且保证断言都不会触发。


https://www.bookstack.cn/read/CPP-Concurrency-In-Action-2ed-2019/content-chapter5-5.3-chinese.md
https://blog.csdn.net/lusic01/article/details/88797658
5-9 可以将sync1和sync2通过在thread_2中使用“读-改-写”操作(memory_order_acq_rel)合并成一个独立的变量。
其中会使用compare_exchange_strong()来保证thread_1对变量只进行一次更新：
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>

using namespace std;

std::atomic<int> data[5];
//std::atomic<bool> sync1(false), sync2(false);
std::atomic<int> sync(0);

void thread_1() {
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141, std::memory_order_relaxed);
    data[4].store(2003, std::memory_order_relaxed);
    //sync1.store(true, std::memory_order_release); // 设置sync1
    sync.store(1, std::memory_order_release); // 设置sync1
    cout<<"sync1 设置成功"<<endl;
}

void thread_2() {
    // while (!sync1.load(std::memory_order_acquire)); // 直到sync1设置后，循环结束
    // sync2.store(true,std::memory_order_release); // 设置sync2
    int  expected=1;
     while(!sync.compare_exchange_strong(expected,2,std::memory_order_acq_rel))
     {
          expected=1;
     }
    cout<<"sync2 设置成功"<<endl;
}

void thread_3() {
    // while (!sync2.load(std::memory_order_acquire)); // 直到sync2设置后，循环结束
    cout<<"thread_3 期待 = "<<sync.load(std::memory_order_acquire) <<endl;
    while(sync.load(std::memory_order_acquire)<2){ cout<<"thread_3 期待 = "<<sync.load(std::memory_order_acquire) <<endl; }
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == -141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
    cout<<"thread3 执行完成"<<endl;
}

int main() {
    std::thread t1(thread_1);
    std::thread t2(thread_2);
    std::thread t3(thread_3);
    t1.join();
    t2.join();
    t3.join();
}
