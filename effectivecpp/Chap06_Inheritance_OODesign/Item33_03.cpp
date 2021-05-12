#include <iostream>
using namespace std;

class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int i)
	{
		cout<<"Base::mf1"<<endl;
	}
	virtual void mf2() 
	{
		cout<<"base mf2 函数"<<endl;
	}

	void mf3();
	void mf3(double dd)
	{
		cout<<"base mf3 函数  dd = "<< dd<<endl;
	}

private:
	int x;
};

class Derived : public Base
{
public:
	virtual void mf1() 
	{
		cout<<"Derived mf1 函数"<<endl;
	}
	void mf3() 
	{
		cout<<"Derived mf3 函数"<<endl;
	}
	void mf4();
	void mf5(double dd)
	{
		cout<<"Derived mf5 函数"<<endl;
		Base::mf3(dd);
	}
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
	d.mf3();
	d.mf5(1.2);

	//可以
	//Base* b = dynamic_cast<Base*>(&d);
	//(*b).mf1(x); 
	
	//错误
    // Derived *pd = new Derived();   
	// pd->mf1(x);

    Base *pd = new Derived();   
	pd->mf1(x);
	return 0;
}
