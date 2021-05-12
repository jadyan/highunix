#include<iostream>
using namespace std;

class Airport
{
};


class Airplane
{
public:
	virtual void fly(const Airport& destination);
};

void Airplane::fly(const Airport& destination)
{
	cout<<"Airplane::fly"<<endl;
	// default code for flyaing an airplane to the given destination.
}


class ModelA : public Airplane
{
};

class ModelB : public Airplane
{
};

int main()
{
	Airport a;

	Airplane* b = new ModelB;
	b->fly(a);

/*
	//找不到fly函数 ModelB隐藏了父类的fly函数
	ModelB bfly;
	bfly.fly();
*/


	//Airplane air;

	delete b;

	return 0;
}