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
		//static_cast<Window>(*this).onResize();
		Window::onResize();
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
