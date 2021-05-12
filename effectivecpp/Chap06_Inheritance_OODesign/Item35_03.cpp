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


// The Template Method Pattern via the Non-Virtual Interface Idiom.
//non-virtual interface
// The Strategy Pattern via Function Pointers.




//将 virtual 函数替换为 ”函数指针的成员变量”；

// Forward declaration
class GameCharacter;

int defaultHealthCalc(const GameCharacter& gc);


class GameCharacter
{
public:
	typedef int (*HealthCalcFunc)(const GameCharacter&);

	explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc) :
		healthFunc(hcf)
	{
	}

	int healthValue() const
	{
        cout<<"GameCharacter::healthValue函数"<<endl;
		return healthFunc(*this);
	}
     virtual void print() const
    {
        cout<<"GameCharacter::print 函数"<<endl;
    }
    virtual ~GameCharacter()
    {
        cout<<"GameCharacter::析构函数"<<endl;
    }
private:
	HealthCalcFunc healthFunc;
};


class EvilBadGuy : public GameCharacter
{
public:
	explicit EvilBadGuy(HealthCalcFunc hcf = defaultHealthCalc) :
		GameCharacter(hcf)
		{
            cout<<"EvilBadGuy构造函数"<<endl;
		}
        virtual void print() const
        {
            cout<<"EvilBadGuy::print 函数"<<endl;
        }
    ~EvilBadGuy()
    {
        cout<<"EvilBadGuy::析构函数"<<endl;
    }
};


// Function for the default health calculation algorithm.
int defaultHealthCalc(const GameCharacter& gc)
{
    gc.print();
	return 0;
}

int main()
{
	//MyHealthCalcFunc mhcf;
	GameCharacter* gc = new EvilBadGuy();
	int h = gc->healthValue();
	delete gc;
	return 0;
}