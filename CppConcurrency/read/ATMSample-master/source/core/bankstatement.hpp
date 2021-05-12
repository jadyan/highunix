/*
 * @Author: your name
 * @Date: 2021-02-04 17:00:05
 * @LastEditTime: 2021-02-08 09:27:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/ATMSample-master/source/core/bankstatement.hpp
 */
//
// Created by Zh1an on 2021/1/21.
//

#ifndef ATMSAMPLE_BANKSTATEMENT_HPP
#define ATMSAMPLE_BANKSTATEMENT_HPP

namespace atm_sample
{


/*银行状态机*/
class bank_machine
{
  messaging::receiver incoming;
  unsigned balance;

public:
  bank_machine() : balance(199) 
  {
    std::cout<<"初始账户余额： "<<balance<<std::endl;
  }

  void done() { get_sender().send(messaging::close_queue()); }

  messaging::sender get_sender() { return incoming; }

  void run()
  {
    try
    {
      for (;;)
      {

        incoming.wait()
            .handle<verify_pin>([&](verify_pin const &msg) {
              if (msg.pin == "1937")
              {
                msg.atm_queue.send(pin_verified());
              }
              else
              {
                msg.atm_queue.send(pin_incorrect());
              }
            })
            .handle<withdraw>([&](withdraw const &msg) {
              if (this->balance >= msg.amount)
              {
                this->balance -= msg.amount;
                msg.atm_queue.send(atm_sample::withdraw_ok());
              }
            })
            .handle<get_balance>([&](get_balance const &msg) { msg.atm_queue.send(atm_sample::balance(balance)); })
            .handle<withdrawal_processed>([&](withdrawal_processed const &msg) {})
            .handle<cancel_withdrawal>([&](cancel_withdrawal const &msg) {});
            //std::cout<<"bank_machine....."<<std::endl;
      }
    }
    catch (messaging::close_queue const &)
    {
    }
  }
};
}  // namespace atm_sample

#endif  // ATMSAMPLE_BANKSTATEMENT_HPP
