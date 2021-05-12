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


//使用 non-virtual interface（NVI）；
// The Template Method Pattern via the Non-Virtual Interface Idiom.
//non-virtual interface
class GameCharacter
{
public:
	// derived classes do not redefine this
	int healthValue() const
	{
        cout<<"GameCharacter::healthValue"<<endl;
		int retVal = doHealthValue();
		return retVal;
	}

private:
	// derived classes may redefine this
	// default algorithm for calc health
	virtual int doHealthValue() const
	{
        cout<<"GameCharacter::doHealthValue 0 "<<endl;
		return 0;
	}
};


class MyCoolCharacter : public GameCharacter
{
private:
	int doHealthValue() const
	{
        cout<<"MyCoolCharacter::doHealthValue 17 "<<endl;
		return 17;
	}
};

int main()
{
	//MyHealthCalcFunc mhcf;
	GameCharacter* gc = new MyCoolCharacter();
	int h = gc->healthValue();
	delete gc;
	return 0;
}