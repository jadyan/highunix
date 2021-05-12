#include <iostream>
#include <string>

#include <string>

class CompanyA
{
public:
	void sendCleartext(const std::string& msg) {}
	void sendEncrypted(const std::string& msg) {}
};


class CompanyB
{
public:
	void sendCleartext(const std::string& msg) {}
	void sendEncrypted(const std::string& msg) {}
};


class MsgInfo {};


template<typename Company>
class MsgSender
{
public:

	void sendClear(const MsgInfo& info)
	{
		std::string msg;
		Company c;
		c.sendCleartext(msg);
	}

	void sendSecret(const MsgInfo& info)
	{
	}
};


/*
class template LoggingMsgSender 定义式时，
并不知道它继承什么样的 class，因为 MsgSender<Company> 不到 class template LoggingMsgSender 被具现化出来时，
是不知道 Company 的样子。也就是说编译器无法确定 sendClear 函数的具体实现，
例如如果 MsgSender<Company> 存在一个特化版，它可能没有对应的 sendClear 函数：\

解决类模板继承问题的办法有三个：

第一个是在 Base class 函数调用动作之前加上 this-> ；
第二个是使用 using 声明式；
第三个是明白指出被调用函数位于 base class 内；
*/
template<typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:
	using MsgSender<Company>::sendClear; // 第二种
	void sendClearMsg(const MsgInfo& info)
	{
		//必须加上this
		sendClear(info);
		//this->sendClear(info);         // 第一种
		//MsgSender<Company>::sendClear(info);    // 第三种
	}
};

int main()
{
	MsgInfo info;
	LoggingMsgSender<CompanyA> log;

	//error C2039: 'sendClearMsg' : is not a member of 'LoggingMsgSender<Company>'
	//log.sendClearMsg(info);

	return 0;
}