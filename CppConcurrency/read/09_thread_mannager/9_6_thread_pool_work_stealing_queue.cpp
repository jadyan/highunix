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
using namespace std;


/*
https://blog.csdn.net/as14569852/article/details/89421365 出现崩溃的原因(可以参考)
*/

// 统一管理线程池所有线程，负责等待线程结束和销毁
class join_threads
{
private:
    std::vector<std::thread> &threads;

public:
    explicit join_threads(std::vector<std::thread> &threads_) : threads(threads_)
    {
    }
    ~join_threads()
    {
        for (unsigned long i = 0; i < threads.size(); ++i)
        {
            // 析构函数中等待线程结束，RAII方式
            if (threads[i].joinable())
                threads[i].join();
        }
    }
};

/*std::packaged_task<>实例是不可拷贝的，仅是可移动的，所以不能再使用std::function<>来实现任务队列
因为std::function<>需要存储可复制构造的函数对象。
这里包装一个自定义函数，用来处理只可移动的类型，就是一个带有函数操作符的类型擦除类
*/
class function_wrapper
{
private:
    struct impl_base
    {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };

    std::unique_ptr<impl_base> impl;

    template <typename F>
    struct impl_type : impl_base
    {
        F f;
        impl_type(F &&f_) : f(std::move(f_)) {}
        void call() { f(); }
    };

public:
    template <typename F>
    function_wrapper(F &&f) : impl(new impl_type<F>(std::move(f)))
    {
    }

    function_wrapper() = default;

    function_wrapper(function_wrapper &&other) : impl(std::move(other.impl))
    {
    }

    function_wrapper &operator=(function_wrapper &&other)
    {
        impl = std::move(other.impl);
        return *this;
    }

    void operator()() { impl->call(); }

    function_wrapper(const function_wrapper &) = delete;
    function_wrapper(function_wrapper &) = delete;
    function_wrapper &operator=(const function_wrapper &) = delete;
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

/*
基于锁的任务窃取队列，代替普通的线程安全队列
该队列支持从前端和后端获取数据，即先进先出和后进先出两种模式，后进先出可以用于其他线程窃取任务
*/
class work_stealing_queue
{
private:
    typedef function_wrapper data_type;
    std::deque<data_type> the_queue; // 使用deque队列保存实际function_wrapper
    mutable std::mutex the_mutex; // 互斥锁用于控制对the_mutex的访问

public:
    work_stealing_queue() {}
    // 拷贝和赋值构造函数不创建
    work_stealing_queue(const work_stealing_queue& other) = delete;
    work_stealing_queue& operator=(const work_stealing_queue& other) = delete;
    // 由互斥锁控制在队列前端插入数据
    void push(data_type data)
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        the_queue.push_front(std::move(data));
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }
    // 由互斥锁控制在队列前端获取数据
    bool try_pop(data_type& res)
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }

        res=std::move(the_queue.front());
        the_queue.pop_front();
        return true;
    }
    // 由互斥锁控制在队列后端获取数据
    bool try_steal(data_type& res)
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }
        res=std::move(the_queue.back());
        the_queue.pop_back();
        return true;
    }
};

class thread_pool4
{
private:
    typedef function_wrapper task_type;

    // 注意这里声明的顺序，是为了保证析构时能以相反的顺序销毁（很重要）
    std::atomic_bool done;
    threadsafe_queue<task_type> pool_work_queue; // 全局任务队列
    std::vector<std::unique_ptr<work_stealing_queue>> queues; // 保存每个线程任务队列的全局队列
    // 保存pool里的工作线程
    std::vector<std::thread> threads;
    // join_threads使用RAII方式，保证pool销毁前所有线程能执行结束
    join_threads joiner;

    //std::atomic_bool start;

    static thread_local work_stealing_queue* local_work_queue; // 每个线程都有一个可以窃取的任务队列，不再使用普通的队列
    static thread_local unsigned my_index;

    void worker_thread(unsigned my_index_) {
        cout << "thread_id: " << this_thread::get_id() << endl;
        my_index = my_index_;
        // 每个线程根据自己的序号从全局队列中获取自己的任务队列。这意味着根据index可以获取任意线程的任务队列
        local_work_queue = queues[my_index].get();
        while (!done)
        {
            run_pending_task();
        }
    }

    bool pop_task_from_local_queue(task_type& task)
    {
        return local_work_queue && local_work_queue->try_pop(task);
    }

    bool pop_task_from_pool_queue(task_type& task)
    {
        return pool_work_queue.try_pop(task);
    }

    bool pop_task_from_other_thread_queue(task_type& task)  // 从其他线程窃取任务
    {

        for(unsigned i=0;i < queues.size();++i)
        {
            // 根据index从其他线程的任务队列后端窃取任务
            unsigned const index = (my_index + i + 1) % queues.size();
            //cout<<"index="<<index<<"  queues[index]="<<&(queues[index])<<endl;
            if(queues[index].get() == this->local_work_queue)
            {
                continue;
            }
            if( !queues[index] && queues[index]->try_steal(task))
            {
                return true;
            }
        }
        return false;

    }

public:
    thread_pool4() : done(false), joiner(threads)
    {
        // pool中线程个数使用硬件支持的最大个数
        unsigned const thread_count = std::thread::hardware_concurrency();

        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                // 当每个线程被创建，就创建了一个属于自己的工作队列，放入全局队列中
                queues.push_back(std::unique_ptr<work_stealing_queue>(new work_stealing_queue));
                // 创建工作线程，每个线程都执行worker_thread函数，在此函数中获取任务处理
                threads.push_back(std::thread(&thread_pool4::worker_thread, this, i));//传入参数i为my_index
            }
        }
        catch (...)
        {
            done = true; // 有异常时，设置done为true
            throw;
        }
        //std::cout<<"start"<<start<<endl;
        //start=true;
        //std::cout<<"start"<<start<<endl;
    }

    // bool getState()
    // {
    //     return start;
    // }
    ~thread_pool4()
    {
        done = true;
    }

    // submit返回一个保存任务返回值的future，使用result_of获取函数functionType的返回类型
    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f)
    {
        // 设置函数functionType的返回类型的别名为result_type
        typedef typename std::result_of<FunctionType()>::type result_type;

        std::packaged_task<result_type()> task(std::move(f)); // 封装一个异步任务，任务执行函数f
        std::future<result_type> res(task.get_future());      // 获取异步任务的future
        if (local_work_queue)                                 // 检查当前线程是否具有一个工作队列，如果有则将任务放入本地队列
        {
            local_work_queue->push(std::move(task)); // 线程本地任务队列
        }
        else
        {
            pool_work_queue.push(std::move(task)); // 将任务添加到全局任务队列中
        }

        return res; // 返回future给submit函数的调用者
    }
    /*
    run_pending_task()的实现去掉了在worker_thread()函数的主循环。
    函数任务队列中有任务的时候执行任务，要是没有的话就会让操作系统对线程进行重新分配。
    */
    void run_pending_task()
    {
        task_type task;
        if (pop_task_from_local_queue(task) || // 第一优先级：从线程自己的队列获取任务
            pop_task_from_pool_queue(task) || // 第二优先级：从线程池队列获取任务
            pop_task_from_other_thread_queue(task)) // 最低优先级：窃取其他线程的任务
        {
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
};


thread_local work_stealing_queue * thread_pool4::local_work_queue;
thread_local unsigned thread_pool4::my_index=0;


template<typename T>
struct sorter {
private:
    thread_pool4 pool;
public:
    list<T> do_sort(list<T> & chunk_data) {
        //等待线程启动完成
        //while(!pool.getState());

        if(chunk_data.empty()) return chunk_data;//If nothing to sort.

        list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());//Copy data to result.

        T const & partition_val = *result.begin();//First element is partition value.
        //order list into two sublists, for first part passing lambda and second part not passing labda.
        typename list<T>::iterator divide_point = partition(chunk_data.begin(), chunk_data.end(), [&](T const & val){return val < partition_val;});

        list<T> new_lower_chunk;//Take first sublist
        new_lower_chunk.splice(new_lower_chunk.begin(), chunk_data, chunk_data.begin(), divide_point);

        //Add recurency this function to worker thread with one sublist and get it future.
        future<list<T>> new_lower = pool.submit(bind(&sorter::do_sort, this, move(new_lower_chunk)));
        list<T> new_higher(do_sort(chunk_data));//Call requrency this function with second sublist.

        result.splice(result.end(), new_higher);//Cummulate result from current thread sublist.
        while(new_lower.wait_for(chrono::seconds(0)) == future_status::timeout)
        {   //While task not done try to do it in current thread.
            pool.run_pending_task();
        }
        //Cummulate result from separated thread sublist and return.
        result.splice(result.begin(), new_lower.get());
        return result;
    }
};

template<typename T>
list<T> parallel_quick_sort(list<T> input) {
    if(input.empty()) return input;

    sorter<T> s;
    return s.do_sort(input);
}

int main()
{
    list<int> l { 198, 111, 34, 424, 56, 65, 2, 5, 3, 33, 46, 53, 64, 77, 87, 98, 556, 54, 78, 45, 87, 79, 88, 99, 5556,
                  564, 7, 8, 0, 678, 242, 465, 876, 43, 655, 5544, 766, 777, 800};
    l = parallel_quick_sort(l);

    for(auto & a : l)
    {
        cout << a << " ";
    }
    cout << endl;

    //std::this_thread::sleep_for(std::chrono::seconds(5));
}
