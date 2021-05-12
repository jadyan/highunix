#include <iostream>
using namespace std;

/*
***
*** 条款35：考虑 virtual 函数以外的其他选择

使用 non-virtual interface（NVI）；
将 virtual 函数替换为 ”函数指针的成员变量”；
以 tr1::function 成员变量替换 virtual 函数；
将继承体系内的 virtual 函数替换为另一个继承体系内的 virtual 函数
*/


//将继承体系内的 virtual 函数替换为另一个继承体系内的 virtual 函数

class GameCharacter;
class HealthCalcFunc
{
public:
	virtual int calc(const GameCharacter& gc) const
	{
		return 17;
	}
};

class MyHealthCalcFunc : public HealthCalcFunc
{
public:
	int calc(const GameCharacter& gc) const
	{
        cout<<"MyHealthCalcFunc::calc "<<" 25"<<endl;
		return 25;
	}
};


HealthCalcFunc defaultHealthCalcFunc;


class GameCharacter
{
public:
	explicit GameCharacter(HealthCalcFunc* phcf = &defaultHealthCalcFunc) :
		pHealthCalcFunc(phcf)
	{
        cout<<"GameCharacter::GameCharacter构造函数"<<endl;
	}

	int healthValue() const
	{
        cout<<"GameCharacter::healthValue()函数"<<endl;
		return pHealthCalcFunc->calc(*this);
	}

private:
	HealthCalcFunc* pHealthCalcFunc;
};


class EvilBadGuy : public GameCharacter
{
public:
	explicit EvilBadGuy(HealthCalcFunc* phcf = &defaultHealthCalcFunc) :
		GameCharacter(phcf)
		{
            cout<<"EvilBadGuy::EvilBadGuy函数"<<endl;
		}
};


int main()
{
	MyHealthCalcFunc mhcf;
	GameCharacter* gc = new GameCharacter(&mhcf);
	int h = gc->healthValue();
	delete gc;
	return 0;
}