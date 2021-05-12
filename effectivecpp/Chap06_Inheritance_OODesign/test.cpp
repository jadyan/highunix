#include <iostream>

class base{
public:
	virtual void fun1()=0;
	virtual void fun2()=0;
	virtual void fun3()=0;
};

void base::fun1()
{
	using namespace std;
	cout<<"fun1"<<endl;
}



int main(){
	 //base b;
}
