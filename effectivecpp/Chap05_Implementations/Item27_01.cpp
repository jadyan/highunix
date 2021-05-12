#include <iostream>
#include <memory>
#include <vector>
//#include "Window4.h"
using namespace std;

class Window
{
public:
	Window()
	{
		cout<<"父类构造"<<endl;
	}
	virtual void onResize() 
	{
		cout<<"父类地址 ox"<<this<<endl;
		cout<<"父类onresize"<<endl;
	}
	virtual ~Window() {}
};

class SpecialWindow : public Window
{
public:
	SpecialWindow()
	{
		cout<<"子类构造"<<endl;
	}
	virtual void onResize()
	{
		cout<<"子类onresize"<<endl;
		cout<<"子类地址 ox"<<this<<endl;
		// this doesn't work!
/*
 *
 *
 * *this 转型为 Window，对函数 onResize 的调用也因此调用了 Window::onResize ，但其实这个函数并没有作用在当前对象身上。
 * 它是在”当前对象的 base class 成分“的副本上调用 Window::onResize ，然后在当前对象身上执行 SpecialWindow 的专属动作。
 * 这使得当前对象进入一种”伤残“状态：其 base class 成分的更改没有落实，而 derived class 成分的更改倒是落实了。
 *
 * 所以如果只是想调用 base class 版本的 onResize 函数，令它作用于当前对象上，可以这样实现：
 *
 * */
		static_cast<Window>(*this).onResize();
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
		(*iter)->onResize();
	}

	return 0;
}
