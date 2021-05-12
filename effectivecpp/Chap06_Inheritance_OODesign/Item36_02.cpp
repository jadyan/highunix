#include <iostream>
using namespace std;


/*
条款36：绝不重新定义继承而来的 non-virtual 函数
*/

class B
{
public:
	virtual void mf()
	{
		cout<<"B:mf"<<endl;
	}

	void mf1()
	{
		cout<<"B:mf1"<<endl;
	}

	void mf2()
	{
		cout<<"B:mf2"<<endl;
	}
};

class D : public B
{
public:
	void mf()
	{
		cout<<"D:mf"<<endl;
	}

	void mf1()
	{
		cout<<"D:mf1"<<endl;
	}
};

int main()
{
	// x is an object of type D.
	D x;

	// pB is statically bound to type B.
	// but: dynamically bound to type D.
	B* pB = &x;

	// call D::mf() through pointer to D.
	pB->mf();


	// pD is statically bound to type D.
	// but: dynamically bound to type D.
	D* pD = &x;

	// call D::mf() through pointer to D.
	pD->mf();




	return 0;
}