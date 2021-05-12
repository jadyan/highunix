/*
 * @Author: your name
 * @Date: 2021-02-22 15:21:51
 * @LastEditTime: 2021-02-23 10:17:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5.4.cpp
 */
//
/*
读操作：memory_order_acquire, memory_order_consume
写操作：memory_order_release
读-修改-写操作：memory_order_acq_rel, memory_order_seq_cst

1. 顺序一致性（Sequential consistency）：对应的内存模型是memory_order_seq_cst

2.请求-释放（Acquire-release）：对应的内存模型是memory_order_consume，memory_order_acquire，memory_order_release，memory_order_acq_rel

3.松散型（非严格约束。Relaxed）：对应的内存模型是memory_order_relaxed


下面对上述3种约束做一个大概解释：

Sequential consistency：指明的是在线程间，建立一个全局的执行序列
Acquire-release：在线程间的同一个原子变量的读和写操作上建立一个执行序列
Relaxed：只保证在同一个线程内，同一个原子变量的操作的执行序列不会被重排序（reorder），这种保证也称之为modification order consistency，但是其他线程看到的这些操作的执行序列式不同的。
还有一种consume模式，也就是std::memory_order_consume。这个模式主要是引入了原子变量的数据依赖。


*/
//  5.4.cpp 同步操作和强制排序->原子操作中的内存顺序
//  排序一致
//  Cpp-Concurrency
//


/*
assert⑤语句是永远不会触发的，因为不是存储x的操作①发生，就是存储y的操作②发生。如果在read_x_then_y中加载
y③返回false，是因为存储x的操作肯定发生在存储y的操作之前，在这种情况下在read_y_then_x中加载x④必定会返回
true，因为while循环能保证在某一时刻y是true。因为memory_order_seq_cst的语义需要一个全序将所有操作都标记为
memory_order_seq_cst，这就暗示着“加载y并返回false③”与“存储y①”的操作，需要有一个确定的顺序。只有在全
序时，当一个线程看到x==true，随后又看到y==false，这就意味着在总序列中存储x的操作发生在存储y的操作之前.


*/

#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>
using namespace std;

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x() {
    x.store(true, std::memory_order_seq_cst); //1 存储x
    cout<<"write_x 执行"<<endl;
}

void write_y() {
    y.store(true, std::memory_order_seq_cst); //2 存储y
    cout<<"write_y 执行"<<endl;
}

void read_x_then_y() {
    while (!x.load(std::memory_order_seq_cst));
    if (y.load(std::memory_order_seq_cst))  //3
    {
        cout<<"read_x_then_y 执行"<<endl;
        ++z;
    }

}

void read_y_then_x() {
    while (!y.load(std::memory_order_seq_cst));
    if (x.load(std::memory_order_seq_cst)){ //4
        cout<<"read_y_then_x 执行"<<endl;
        ++z;
    }

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
    assert(z.load() != 0); //5 永远不会触发，因为不是存储x, 就是y
}
