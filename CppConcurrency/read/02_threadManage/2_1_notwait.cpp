/*
 * @Author: your name
 * @Date: 2021-01-22 17:46:32
 * @LastEditTime: 2021-01-26 16:11:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.1.cpp
 */
//
//  2.1.cpp 不等待线程的问题


/*
这个例子中，已经决定不等待线程结束(使用了detach() ② )，
所以当oops()函数执行完成时③，新线程中的函数可能还在运行。
如果线程还在运行，它就会去调用do_something(i)函数①，这时就会访问已经销毁的变量。
如同一个单线程程序——允许在函数完成后继续持有局部变量的指针或引用；
当然，这从来就不是一个好主意——这种情况发生时，错误并不明显，会使多线程更容易出错。运行顺序参考表2.1。
*/

#include <iostream>
#include <thread>

using  namespace std;

void do_something(int& i) {
    cout<<"线程函数"<<endl;
    static int y=0;
    std::cout<<"y ="<<y++<<std::endl;
    ++i;
}

struct func {
    int& i;
    func(int& i_):i(i_) {}
    void operator()() {
        std::cout<<"线程开始"<<endl;
        for (unsigned j = 0; j < 1000000; j++) {
            do_something(i); // 存在隐患，悬空引用
        }
    }
};

void opps() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach(); // 不等待线程结束
    std::cout<<"主线程结束"<<std::endl;
}                       // 新线程可能还在运行

int main() {
    opps();
}
