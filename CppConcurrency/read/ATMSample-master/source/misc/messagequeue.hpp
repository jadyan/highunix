/*
 * @Author: your name
 * @Date: 2021-02-04 16:04:44
 * @LastEditTime: 2021-02-04 16:51:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/ATMSample-master/source/misc/messagequeue.hpp
 * 
 * 
 * 
 * 实现了一个消息队列。其可以将消息以指针(指向基类)的方式存储在列表中；指定消息类型会由基类派生模板进行处理。推送包装类的构造实例，以及存储指向这个实例的指针；弹出实例的时候，将会返回指向其的指针。因为message_base类没有任何成员函数，在访问存储消息之前，弹出线程就需要将指针转为wrapped_message<T>指针。
 */


#ifndef ATMSAMPLE_MESSAGEQUEUE_HPP
#define ATMSAMPLE_MESSAGEQUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace messaging
{
struct message_base
{  // 队列项目的基类
  virtual ~message_base() = default;
};

template <typename Msg>
struct wrapped_message : message_base
{  // 消息类型有特殊的定义
  Msg contents;
  explicit wrapped_message(Msg const &contents_) : contents(contents_) {}
};

class queue
{  // 消息队列
  std::mutex mutex;
  std::condition_variable conditionVariable;
  std::queue<std::shared_ptr<message_base>> q;  // 实际的队列存储 message_base 指针
public:
  template <typename T>
  void push(T const &msg)
  {
    std::lock_guard<std::mutex> lk(mutex);
    q.push(std::make_shared<wrapped_message<T>>(msg));
    conditionVariable.notify_all();
  }

  std::shared_ptr<message_base> wait_and_pop()
  {
    std::unique_lock<std::mutex> lk(mutex);
    conditionVariable.wait(lk, [&] { return !q.empty(); });
    auto res = q.front();
    q.pop();
    return res;
  }
};
};  // namespace messaging

#endif  // ATMSAMPLE_MESSAGEQUEUE_HPP
