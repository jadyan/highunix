/*
 * @Author: your name
 * @Date: 2021-02-01 09:06:06
 * @LastEditTime: 2021-02-01 10:11:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4_1_condition.cpp
 */
//



//  4.1.cpp 等待一个事件或其他事件->等待条件达成
//  Cpp-Concurrency

//  使用 std::condition_variable 处理数据等待

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <unistd.h>

using namespace std;

bool more_data_to_prepare() {
    //return false;
    return true;
}

struct data_chunk {
    data_chunk(const string &str)
    {
        m_str=str;
        //cout<<"m_str = "<<m_str<<endl;
    }
    void show()
    {
        cout<<"get ... m_str"<<m_str<<endl;
    }
    private:
     string m_str;
};

data_chunk prepare_data() {
    static int count=0;

    string  str =to_string(count)+ ". add";
    cout<<"set str = "<<str<<endl;
    if (count>100){
        count=0;
    }
    count++;

    return data_chunk(str);
}

void process(data_chunk& data) {
    data.show();
}

bool is_last_chunk(data_chunk&) {
    //return true;
    return false;
}

std::mutex mut;
std::queue<data_chunk> data_queue; // 两个线程之间传递数据的队列
std::condition_variable data_cond;

void data_preparation_thread() {
    cout<<"data_preparation_thread start..."<<endl;
    while (more_data_to_prepare()) {
        data_chunk const data = prepare_data();
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data); // 准备好的数据放入队列
        data_cond.notify_one(); // 通知等待线程
        //cout<<"11."<<endl;
        sleep(1);
    }
}


/*
为什么用std::unique_lock而不使用std::lock_guard——等待中的线程必须在等待期间解锁互斥量，
并在这之后对互斥量再次上锁，而std::lock_guard没有这么灵活。
如果互斥量在线程休眠期间保持锁住状态，准备数据的线程将无法锁住互斥量，也无法添加数据到队列中；同样的，等待线程也永远不会知道条件何时满足。
*/
void data_processing_thread() {
    //sleep (2);
    cout<<"data_processing_thread start..."<<endl;
    while (true) {
        std::unique_lock<std::mutex> lk(mut); // 这里 unique_lock 比 lock_guard 好
                                              // 因为等待中的线程必须在等待期间解锁互斥量，并在这之后对互斥量再次上锁
                                              // 所以需要用灵活的 unique_lock，如果在休眠期间锁不能解开就不能满足以上条件
        //cout<<"1."<<endl;
        //wait()函数将解锁互斥量，并且将这个线程(上段提到的处理数据的线程)置于阻塞或等待状态。
        data_cond.wait(lk, [] { return !data_queue.empty(); }); // wait传递一个锁 和 lambda->检查队列是否为空
        //cout<<"2."<<endl;
        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock(); // 解锁不仅可以用于wait，还可以用于待处理的数据

        sleep (1);
        process(data);
        if (is_last_chunk(data)){
            break;
        }

    }
}

int main() {
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}
