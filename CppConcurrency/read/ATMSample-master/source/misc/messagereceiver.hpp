/*
 * @Author: your name
 * @Date: 2021-02-08 10:03:29
 * @LastEditTime: 2021-02-08 10:10:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/ATMSample-master/source/misc/messagereceiver.hpp
 */
//
// Created by Zh1an on 2021/1/21.
//

#ifndef ATMSAMPLE_MESSAGERECEIVER_HPP
#define ATMSAMPLE_MESSAGERECEIVER_HPP

#include <misc/messagedispatcher.hpp>

namespace messaging
{

//sender只是引用一个消息队列，而receiver是拥有一个队列。
//可以使用隐式转换的方式获取sender引用的类。
//难点在于wait()中的调度。这里创建了一个dispatcher对象引用receiver中的队列。
class receiver
{
  queue q;  // 一个 receiver 拥有此队列

public:
  operator sender()
  {  // 允许隐式转换到引用队列的 sender
    return sender(&q);
  }
  dispatcher wait()
  {  // 等待队列创建调度器
    return dispatcher(&q);
  }
};
}  // namespace messaging

#endif  // ATMSAMPLE_MESSAGERECEIVER_HPP
