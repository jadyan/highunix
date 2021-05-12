#include <iostream>
using namespace std;

class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int i) {}
	virtual void mf2()
	{
		cout<<"base mf2 函数"<<endl;
	}

	void mf3() 
	{
		cout<<"base mf3 函数"<<endl;
	}
	void mf3(double d) 
	{
		cout<<"base mf3 double函数"<<endl;
	}

private:
	int x;
};

class Derived : public Base
{
public:
	// Make all things in Base named mf1 and mf3
	// visible (and public) in Derived's scope
	using Base::mf1;
	using Base::mf3;

	virtual void mf1() {}
	void mf3() 
	{
		cout<<"Derived mf3 函数"<<endl;
	}
	void mf4();
};

void Derived::mf4()
{
	cout<<"Derived mf4  函数"<<endl;	
}


int main()
{
	Derived d;
	const int x = 5;

	d.mf1();		
	//d.mf1(x);	
	d.mf3();	
	d.mf3(1.2);
	return 0;
}