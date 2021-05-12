
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <chrono>
#include <list>
#include <future>         // std::promise, std::future

#include <boost/shared_ptr.hpp>


template <typename T>
class thread_safe_stack {
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T> > data_queue;
    std::condition_variable data_cond;
public:
    thread_safe_stack() {}

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(*data_queue.front());  //弹出函数会持有一个变量的引用，为了接收这个新值，必须对存储的指针进行解引用
        data_queue.pop();
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(*data_queue.front()); //弹出函数会持有一个变量的引用，为了接收这个新值，必须对存储的指针进行解引用 
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    void push(T new_value) {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        //std::shared_ptr<T> data(std::make_shared<T>(new_value));
        //std::shared_ptr<T> data(new T(new_value));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
};



template <typename T>
struct sorter // 1
{
    struct chunk_to_sort
    {
        std::list<T> data;
        std::promise<std::list<T>> promise;
    };

    thread_safe_stack<chunk_to_sort> chunks; // 2 使用线程安全栈保存data数据块
    std::vector<std::thread> threads;       // 3 保存线程
    unsigned const max_thread_count;        //最大支持的线程数，根据硬件获得
    std::atomic<bool> end_of_data;

    sorter() : max_thread_count(std::thread::hardware_concurrency() - 1),
               end_of_data(false)
    {
    }

    ~sorter() // 4
    {
        end_of_data = true; // 5

        for (unsigned i = 0; i < threads.size(); ++i)
        {
            threads[i].join(); // 6
        }
    }
    //从线程安全栈中获取一个数据块，进行排序
    void try_sort_chunk()
    {
        std::shared_ptr<chunk_to_sort> chunk = chunks.try_pop(); // 7
        if (chunk)
        {
            sort_chunk(chunk); // 8
        }
    }
    //排序的主函数
    std::list<T> do_sort(std::list<T> &chunk_data) // 9
    {
        if (chunk_data.empty())
        {
            return chunk_data;
        }

        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin()); //转移数据到result列表中
        T const &partition_val = *result.begin();

        /*
        std::partition会将区间[first,last)中的元素重新排列，满足判断条件pred的元素会被放在区间的前段，不满足pred的元素会被放在区间的后段。该算法不能保证元素的初始相对位置，如果需要保证初始相对位置，应该使用stable_partition.
        */
        //10 对data进行第一次快速排序，根据data第一个元素排序，表达式true放左边，false放右边，返回值为中间分组点
        typename std::list<T>::iterator divide_point = std::partition(chunk_data.begin(),
                                                                      chunk_data.end(),
                                                                      [&](T const &val) { return val < partition_val; });

        chunk_to_sort new_lower_chunk; //定义新的data类型保存上面排序结果的左部分数据块
        new_lower_chunk.data.splice(new_lower_chunk.data.end(),
                                    chunk_data, chunk_data.begin(),
                                    divide_point);

        std::future<std::list<T>> new_lower = new_lower_chunk.promise.get_future();
        chunks.push(std::move(new_lower_chunk)); // 11 将上面的左部分数据块保存到线程安全栈中
        if (threads.size() < max_thread_count)   // 12
        {                                        //启动新线程处理左部分数据块的排序
            threads.push_back(std::thread(&sorter<T>::sort_thread, this));
        }

        std::list<T> new_higher(do_sort(chunk_data)); //在本线程继续处理右部分的排序

        result.splice(result.end(), new_higher);
        //等下期望值变为ready，即表示新线程的排序任务已经结束
        while (new_lower.wait_for(std::chrono::seconds(0)) != std::future_status::ready) // 13
        {
            try_sort_chunk(); // 14 如果没有结束，本线程等待，并尝试处理安全栈中数据
        }

        result.splice(result.begin(), new_lower.get());
        return result;
    }
    //对数据进行排序，并把结果保存在承诺值中
    void sort_chunk(std::shared_ptr<chunk_to_sort> const &chunk)
    {
        chunk->promise.set_value(do_sort(chunk->data)); // 15
    }

    void sort_thread()
    {
        while (!end_of_data) // 16
        {
            try_sort_chunk();          // 17
            std::this_thread::yield(); // 18
        }
    }
};

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) // 19 并行快速排序函数
{
    if (input.empty())
    {
        return input;
    }
    sorter<T> s;

    return s.do_sort(input); // 20
}




int main(int argc, char** argv)
{

    std::list<int> mylist;
    mylist.push_back(9); 
    mylist.push_back(7); 
    mylist.push_back(8); 

std::cout<<"排序前"<<std::endl;
    for (auto v: mylist)
    {
        std::cout<<v<<std::endl;
    }
    std::list<int> mylist2;
    mylist2 = parallel_quick_sort(mylist);

std::cout<<"排序后"<<std::endl;
    for (auto v: mylist2)
    {
        std::cout<<v<<std::endl;
    }

    return 0;
}