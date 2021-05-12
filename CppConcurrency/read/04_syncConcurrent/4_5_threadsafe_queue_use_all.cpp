/*
 * @Author: your name
 * @Date: 2021-02-01 16:01:47
 * @LastEditTime: 2021-02-01 16:02:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.5.cpp
 */


//  4.5.cpp 等待一个事件或其他事件->使用条件变量构建线程安全队列
//  Cpp-Concurrency
//
//  使用条件变量的线程安全队列

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <thread>
#include <unistd.h>


using namespace std;

template <typename T>
class threadsafe_queue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue() {}
    threadsafe_queue(threadsafe_queue const& other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); } );
        value = data_queue.front();
        data_queue.pop();
    }
    
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); } );
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
    }
    
    std::shared_ptr<T> try_pop() {
        std::unique_lock<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

struct data_chunk {
    data_chunk(const string &str)
    {
        m_str=str;
        //cout<<"m_str = "<<m_str<<endl;
    }
    data_chunk()
    {
        m_str="";
        //cout<<"m_str = "<<m_str<<endl;
    }
    void show()
    {
        cout<<"get ... m_str"<<m_str<<endl;
    }
    private:
     string m_str;
    
};

data_chunk prepare_data()
{
    static int count=0;

    string  str =to_string(count)+ ". add";
    cout<<"set str = "<<str<<endl;
    if (count>100){
        count=0;
    }
    count++;

    return data_chunk(str);
};
bool more_data_to_process()
{
    return true;
}
void process(data_chunk & data)
{
    data.show();
}
bool is_last_chunk(data_chunk & data)
{
    return false;
}

threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread() {
    while (more_data_to_process()) {
        data_chunk const data = prepare_data();
        data_queue.push(data);
        sleep(1);
    }
}

void data_processing_thread() {
    while (true) {
        //sleep(1);
        data_chunk data;
        data_queue.wait_and_pop(data);
        process(data);
        if (is_last_chunk(data))
            break;
    }
}


int main(int argc, char **argv)
{
    std::thread T1(data_preparation_thread);
    std::thread T2(data_processing_thread);
    T1.join();
    T2.join();
    return 0;
}
