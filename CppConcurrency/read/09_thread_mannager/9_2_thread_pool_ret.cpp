#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <thread>
#include <vector>
#include <list>
#include <future>
#include <exception>
#include <numeric>
#include <mutex>
#include <atomic>
#include <chrono>
#include <exception>
#include <queue>
#include <condition_variable>
#include <chrono>
using namespace std;

/* 
执行任务的线程需要返回一个结果到主线程上进行处理。可以使用submit()函数提交任务并返回条件变量或future，用来等待任务的完成，返回任务返回值。
在上面例子的基础上，使用packged_task和future代替std::function，可以返回任务返回值：
*/
class function_wrapper {
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base(){}
    };

    unique_ptr<impl_base> impl;

    template<typename F>
    struct impl_type : impl_base {
        F f;
        impl_type(F && f_) : f(move(f_)){}
        void call() {
            f();
        }
    };

public:
    template<typename F>
    function_wrapper(F && f): impl(new impl_type<F>(move(f))) {}

    void operator()() {
        impl->call();
    }

    //Move semantics allowed.
    function_wrapper() = default;
    function_wrapper(function_wrapper && other) : impl(move(other.impl)) {}
    function_wrapper & operator=(function_wrapper && other) {
        impl = move(other.impl);
        return *this;
    }

    //Copy semantics not allowed.
    function_wrapper(const function_wrapper &) = delete;
    function_wrapper(function_wrapper&) = delete;
    function_wrapper & operator=(const function_wrapper &) = delete;
};

template<typename T>
class threadsafe_queue {
private:
    mutable mutex mut;
    queue<shared_ptr<T>> data_queue;
    condition_variable data_cond;
public:
    threadsafe_queue(){}

    void push(T new_value) {
        shared_ptr<T> data = make_shared<T>(move(new_value));
        lock_guard<mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    void wait_and_pop(T & value) {
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk, [this](){return !data_queue.empty();});
        value = move(*data_queue.front());
        data_queue.pop();
    }

    shared_ptr<T> wait_and_pop() {
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk, [this](){return !data_queue.empty();});
        shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool try_pop(T & value) {
        lock_guard<mutex> lk(mut);
        if(data_queue.empty()) {
            return false;
        }
        value = move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    shared_ptr<T> try_pop() {
        lock_guard<mutex> lk(mut);
        if(data_queue.empty()) {
            return shared_ptr<T>();
        }
        shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool empty() const {
        lock_guard<mutex> lk(mut);
        return data_queue.empty();
    }
};

class join_threads
{
    vector<thread> & threads;
public:
    explicit join_threads(vector<thread> & threads_) : threads(threads_){}

    ~join_threads() {
        for(unsigned long i = 0; i < threads.size(); ++i) {
            if(threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};

class thread_pool
{
    atomic_bool done;
    //Own function_wrapper for tasks to be done in thread_pool instead of
    //std::function<> to accept std::packaged_task<>.
    threadsafe_queue<function_wrapper> work_queue;
    vector<thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while(!done) {
            function_wrapper task;
            if(work_queue.try_pop(task)) {
                task();
            } else {
                this_thread::yield();
            }
        }
    }

public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = thread::hardware_concurrency();

        try {
            for(unsigned i = 0; i < thread_count; i++) {
                threads.push_back(thread(&thread_pool::worker_thread, this));
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    ~thread_pool() {
        done = true;
    }

#if 0
    //Submit task to pool thread and get future associated with the result
    //of this task.
    //返回一个 std::future<>保存任务的返回值，并允许调用者等待任务完全结束
    template<typename FunctionType>
    future<typename result_of<FunctionType()>::type> submit(FunctionType f) {
        //对result_type 使用 std::result_of
        typedef typename result_of<FunctionType()>::type result_type;
        //Create packaged_task based on submited function. Wrap this function.
        //将f包装入std::packaged_task<result_type()>
        packaged_task<result_type()> task(move(f));
        //Get future asociated with the result of this task.
        //从 std::packaged_task<>中获取future
        future<result_type> res(task.get_future());
        // 向任务队列推送任务
        work_queue.push(move(task));//Move task to queue.
        return res;//Return result.
    }
#else
    template<typename FunctionType>
    // 返回一个 std::future<>保存任务的返回值，并允许调用者等待任务完全结束
    std::future<typename std::result_of<FunctionType()>::type> 
    submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // 对 result_type 使用 std::result_of
        
        std::packaged_task<result_type()> task(std::move(f)); // 将f包装入std::packaged_task<result_type()>
        std::future<result_type> res(task.get_future()); // 从 std::packaged_task<>中获取future
        work_queue.push(std::move(task)); // 向任务队列推送任务
        return res; // 返回future
    }
#endif

};

int f1() {
    cout << "f1" << endl;
    this_thread::sleep_for(std::chrono::seconds(1));
    return 1;
}

int f2() {
    cout << "f2" << endl;
    this_thread::sleep_for(std::chrono::seconds(1));
    return 2;
}

double f3() {
    cout << "f3" << endl;
    this_thread::sleep_for(std::chrono::seconds(1));
    return 3.23;
}

int main()
{
    thread_pool tp;
    this_thread::sleep_for(std::chrono::seconds(1));
    cout << "------------------------------" << endl;

    auto r1 = tp.submit(f1);
    cout << r1.get() << endl;
    cout << "------------------------------" << endl;

    auto r2 = tp.submit(f1);
    auto r3 = tp.submit(f2);
    cout << r2.get() << endl;
    cout << r3.get() << endl;
    cout << "------------------------------" << endl;

    auto r4 = tp.submit(f1);
    auto r5 = tp.submit(f2);
    auto r6 = tp.submit(f3);
    cout << r4.get() << endl;
    cout << r5.get() << endl;
    cout << r6.get() << endl;
    cout << "------------------------------" << endl;

    auto r7 = tp.submit(f1);
    auto r8 = tp.submit(f2);
    auto r9 = tp.submit(f3);
    auto r10 = tp.submit([](){cout << "f4" << endl; this_thread::sleep_for(std::chrono::seconds(1)); return 4;});
    cout << r7.get() << endl;
    cout << r8.get() << endl;
    cout << r9.get() << endl;
    cout << r10.get() << endl;
    cout << "------------------------------" << endl;
}