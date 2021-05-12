#include <iostream>
using namespace std;

class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf2() 
	{
		cout<<"base mf2  函数"<<endl;	
	}
	void mf3();

private:
	int x;
};

class Derived : public Base
{
public:
	virtual void mf1() 
	{
		cout<<"Derived mf1  函数"<<endl;	
	}
	void mf4();
};


void Derived::mf4()
{
	cout<<"Derived mf4  函数"<<endl;	
	mf2();
}


int main()
{
	Derived d;
	const int x = 5;

	d.mf1();		// Fine: calls Derived::mf1
	//d.mf1(x);		// Error Base::mf1() is hidden.
	d.mf4();
	return 0;
}