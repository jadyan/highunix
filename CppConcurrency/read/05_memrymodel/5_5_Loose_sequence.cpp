/*
 * @Author: your name
 * @Date: 2021-02-22 15:47:37
 * @LastEditTime: 2021-02-23 09:55:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_5_Loose_sequence.cpp
 */
//
//  5.5.cpp 同步操作和强制排序->原子操作中的内存顺序
//  自由排序
//  Cpp-Concurrency
//

/*
https://my.oschina.net/SHIHUAMarryMe/blog/805489
std::memory_order_relaxed(自由序列)
该模式下仅仅保证了读写的完整性，且还有要求是单个线程内的同一个原子类型的所有操作不能进行乱序.也就是说
对一个多线程下的变量进行读写的时候，保证一定会读到完整的值（不会出现读到一个不完整的值,
至于该值是新值还是旧值，是没法保证的，同步几次 和 何时同步 CPU说了算），
写的时候仅仅保证值一定会被完整的写入(寄存器),至于何时同步,以及对是否在每次写入结束后立即同步到内存中就要看CPU了.

解析
assert是仍然有可能触发的! 首先我们需要注意到的是无论是read操作(load)，还是write(store),我们都指定内存
模型为std::memory_order_relaxed,这样一来也就意味着1和2处的操作有可能被乱序执行，5,6和7处也一样可
能被乱序执行。不仅仅是乱序这么简单1, 2, 5, 6, 7处的写操作很可能都仅仅写入了高速缓存/寄存器，也可能并
没有同步内存中的，那么在3和4处执行的read操作就可能即使y load到了true, 而x仍然load到false.
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>


using namespace std;

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed); //1
    y.store(true, std::memory_order_relaxed); //2
    cout<<"write_x_then_y 执行"<<endl;

}

void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed));  //3
    if (x.load(std::memory_order_relaxed)) // 4 可能读到false
    {
        ++z;
        cout<<"read_y_then_x 执行"<<endl;
    }

}

int main() {
    x = false;  //5
    y = false;  //6
    z = 0;      //7
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); // 可能会触发
}
