/*****
 *
 *条款27：尽量少做转型
 *
 * C++ 规则的设计目标之一是保证类型错误绝不可能发生。但是程序中避免不了转型（casts）操作，而转型会破坏类型系统（type system）
 *
 *
 * *****/
#include <iostream>
#include <memory>
#include <vector>
//#include "Window4.h"
using namespace std;

class Window
{
public:
	virtual void blink() 
	{
		cout<<"父类 blink"<<endl;
	}
};


class SpecialWindow : public Window
{
public:
	virtual void blink()
	{
		cout<<"子类 blink函数"<<endl;
		int x = 7;
	}
};




int main()
{
	typedef std::vector<std::shared_ptr<Window> > VPW;
	VPW winPtrs;
	
	shared_ptr<Window> ptr(new SpecialWindow());
	winPtrs.push_back(ptr);
	

	for (VPW::iterator iter = winPtrs.begin(); iter != winPtrs.end(); ++iter)
	{
		(*iter)->blink();
	}

	return 0;
}
