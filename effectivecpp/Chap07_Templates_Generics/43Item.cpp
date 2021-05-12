#include <iostream>
#include <string>

/*
Item 43: 了解如何访问 templatized base classes（模板化基类）中的名字
在 derived class templates（派生类模板）中，可以经由 "this->" 前缀，
经由 using declarations，或经由一个 explicit base class qualification（显式基类限定）
引用 base class templates（基类模板）中的名字
*/


class CompanyZ
{
public:
	void sendEncrypted(const std::string& msg) {}
};


class MsgInfo {};


// http://stackoverflow.com/questions/5510922/template-class-specialization
template<typename T>
class MsgSender
{
};


/*
模板特化
*/
template<>
class MsgSender<CompanyZ>
{
public:
	void sendSecret(const MsgInfo& info)
	{
	}

    void sendClear(const MsgInfo& info)
	{
		std::string msg;
		// Company c;
		// c.sendCleartext(msg);
	}
};


template<typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:

	void sendClearMsg(const MsgInfo& info)
	{
		this->sendClear(info);
	}
};

int main()
{
	MsgInfo info;
	LoggingMsgSender<CompanyZ> log;

	//error C2039: 'sendClearMsg' : is not a member of 'LoggingMsgSender<Company>'
	log.sendClearMsg(info);

	return 0;
}