/*
 * @Author: your name
 * @Date: 2021-02-23 09:12:58
 * @LastEditTime: 2021-02-23 10:06:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_7_relaxed_ordering.cpp
 */
//
//  5.7.cpp 同步操作和强制排序->原子操作中的内存顺序
//  获取-释放序列
//  Cpp-Concurrency
//
/*
std::memory_order_release 和 std::memeory_order_acquire(注意这两个一定是成对出现的).

该模型下不仅仅提供了保证读写的完整性，保证同一个线程内的同一个原子变量的所有操作不能乱序，而且提供了同步（这个同步可厉害了见下文)！

如果有一个atomic变量，在线程A中atomic store(taged std::memory_order_release)一个值到该变量中去，
在线程B中atomic load(taged std::memory_order_acquire)atomic 变量中的值,当atomic 变量的load(taged std::memory_order_acquire)操作完成后,
所有发生在线程A中atomic store(taged std::memory_order_release)操作之前的
其他变量(普通变量以及store taged std::memory_order_relaxed)写入的值(产生的side effect)
对于线程B中atomic 变量load(taged std::memory_order_acquire)之后的操作均可见
(可以把线程B中atomic 变量load taged std::memory_order_acquire理解为一个同步点
但是这个同步点不仅仅同步了该atomic变量也同步了其他变量(其他变量包括普通变量以及taged std::memory_order_relaxed的atomic变量)!

比如:

线程 A 原子性地把值写入 x (release), 然后线程 B 原子性地读取 x 的值（acquire）.
 这样线程 B 尽可能的读取到 x 的最新值。注意 release - acquire 有个牛逼的副作用：线程 A 中所有发生在 release x 之前的写操作，
 对在线程 B acquire x 之后的任何读操作都可见！本来 A, B 间读写操作顺序不定。这么一同步，
 在 x 这个点前后， A, B 线程之间有了个顺序关系，称作: inter-thread happens-before.



 上面的例子中 x 和 y 都是由不同的线程写入的，因此在 线程C和D 中：1和3 的load(acqiire)操作由于是位于
 while中因此可以多次进行同步(如果一次不成功就再同步一次直到load出来的值为true为止). 但是3和4的
 load(acquire)操作可能会load到false,因为 x.store和y.store 操作是作为 两个线程(A和B) 来执行的,所以在线程C
 中 1处的操作只能看到 线程A(线程A必须早于线程C时)中的 store(release) 的值,至于 线程C 中 2处的操作 可能
 由于CPU的乱序执行 造成运行到了 2处 而 线程B 却还没执行（或者刚刚开始执行，或者执行一半）这样一来看
 到就还是false; 线程D中同理.
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x() {
    x.store(true, std::memory_order_release);
}

void write_y() {
    y.store(true, std::memory_order_release);
}

void read_x_then_y() {
    while (!x.load(std::memory_order_acquire)); //1
    if (y.load(std::memory_order_acquire))      //2
    {
        std::cout<<"read_x_then_y"<<std::endl;
        ++z;
    }
}

void read_y_then_x() {
    while (!y.load(std::memory_order_acquire));  //3
    if (x.load(std::memory_order_acquire))       //4
    {
        std::cout<<"read_y_then_x"<<std::endl;
        ++z;
    }

}

void print()
{
    std::cout<<"z="<<z<<std::endl;
}
int main() {
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0); //5 可能触发
    print();
}

