/*
 * @Author: your name
 * @Date: 2021-02-04 16:39:30
 * @LastEditTime: 2021-02-05 10:13:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/ATMSample-master/source/misc/messagesender.hpp
 */
//
// Created by Zh1an on 2021/1/21.
//

#ifndef ATMSAMPLE_MESSAGESENDER_HPP
#define ATMSAMPLE_MESSAGESENDER_HPP

#include <misc/messagequeue.hpp>

namespace messaging
{
class sender
{
  queue *q;  // sender 就是封装了队列指针
public:
  sender() : q(nullptr) {}  // 默认构造的 sender 没有队列
  explicit sender(queue *q_) : q(q_) {}  // 允许从指向队列的指针进行构造
  template <typename myMessage>
  void send(myMessage const &msg)
  {
    //std::cout<<"send****"<<std::endl;
    if (q)
    {
      q->push(msg);
    }
  }
};
}  // namespace messaging

#endif  // ATMSAMPLE_MESSAGESENDER_HPP
