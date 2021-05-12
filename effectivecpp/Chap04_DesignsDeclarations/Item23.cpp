#include <iostream>


/*
 *
 *条款23：宁以 non-member、non-friend 替换 member 函数
		 Prefer non-member non-friend functions to member functions.
 *
 * */

namespace WebBrowserStuff
{
	class WebBrowser
	{
	public:
		void clearCache() { std::cout<<"claerCache"<<std::endl;}
		void clearHistory() { std::cout<<"clearHistory"<<std::endl; }
		void removeCookies() {std::cout<<"removeCookies"<<std::endl;  }
		//void clearEverything(); // 调用 clearCache，clearHistory，removeCookies；
	};
}



namespace WebBrowserStuff
{
	void clearBrowser(WebBrowser& wb)
	{
		wb.clearCache();
		wb.clearHistory();
		wb.removeCookies();
	}
}



using namespace WebBrowserStuff;
int main(int argc, char ** argv)
{
	WebBrowser wb;
	clearBrowser(wb);
	return 0;
}

