/*
 * @Author: your name
 * @Date: 2021-02-03 10:32:02
 * @LastEditTime: 2021-02-03 14:43:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4_9_thread_send_task.cpp
 * 
 * 很多图形架构需要特定的线程去更新界面，所以当一个线程对界面的更新时，
 * 它要发出一条信息给正确的线程，让特定的线程来做界面更新。std::packaged_task提供了实现这种功能的方法，
 * 且不需要发送一条自定义信息给图形界面相关线程。下面来看看代码
 * 
 * 
 * 这段代码十分简单：图形界面线程①循环直到收到一条关闭图形界面的信息后关闭②，进行轮询界面消息处理③，例如：
 * 用户点击和执行在队列中的任务。当队列中没有任务④，它将再次循环；除非，它能在队列中提取出一个任务⑤，然后释
 * 放队列上的锁，并且执行任务⑥。这里，期望值与任务相关，当任务执行完成时，其状态会被置为“就绪”状态。
 * 
 * http://www.voidcn.com/article/p-dioigoxp-sq.html
 */


#include <iostream>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include <chrono>

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

static int shutdown = 3;

bool gui_shutdown_message_received()
{
	// 执行3次则结束GUI线程 
	return ((shutdown--) < 0);
}
void get_and_process_gui_message()
{
	
}

// 倒计时3秒 
void countdown()
{
	int from = 3, to = 0;
  	for (int i=from; i!=to; --i)
	{
    	std::cout << i << '\n';
    	std::this_thread::sleep_for(std::chrono::seconds(1));
  	}
  	std::cout << "Lift off!\n";
}

// GUI线程
void gui_thread()
{
	// 循环直到收到结束消息 
    while(!gui_shutdown_message_received())
    {
    	std::cout << "gui_thread" << std::endl;
    	// 获取GUI消息并处理 
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            // 如果队列中没有任务，执行下一次循环 
            if(tasks.empty())
                continue;
            // 从队列中获取任务 
            task=std::move(tasks.front());
            tasks.pop_front();
        }
        // 释放mutex，执行任务 
        task();
    }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::cout << "post_task_for_gui_thread" << std::endl;
	// 创建packaged_task，任务函数为countdown 
    std::packaged_task<void()> task(f);
    // 从任务获取期望 
    std::future<void> res=task.get_future();
    std::lock_guard<std::mutex> lk(m);
    // 将任务加入到队列中 
    tasks.push_back(std::move(task));
    return res;
}

int main()
{
	std::cout << "main" << std::endl;
	
	std::future<void> f = post_task_for_gui_thread(countdown);
	std::cout << "posted 1" << std::endl;
	
	post_task_for_gui_thread(countdown);
	std::cout << "posted 2" << std::endl;

	// 等待第一个GUI线程的任务执行完成 
	f.get();
	std::cout << "got" << std::endl;
	
    gui_bg_thread.join();

}