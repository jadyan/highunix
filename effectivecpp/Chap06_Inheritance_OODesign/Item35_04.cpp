#include <iostream>
#include <functional>
using namespace std;

/*
***
*** 条款35：考虑 virtual 函数以外的其他选择

使用 non-virtual interface（NVI）；
将 virtual 函数替换为 ”函数指针的成员变量”；
以 function 成员变量替换 virtual 函数；
将继承体系内的 virtual 函数替换为另一个继承体系内的 virtual 函数
*/


// The Template Method Pattern via the Non-Virtual Interface Idiom.
//non-virtual interface
// The Strategy Pattern via Function Pointers.




// The Strategy Pattern via function.



//以 function 成员变量替换 virtual 函数；

// Forward declaration
class GameCharacter;

// Function for the default health calculation algorithm.
int defaultHealthCalc(const GameCharacter& gc);



class GameCharacter
{
public:
	// HealthCalcFunc is any callable entity that can be called with
	// anything compatible with a GameCharacter and that returns
	// anything compatible with an int; see below for details.
	typedef std::function<int (const GameCharacter&)> HealthCalcFunc;

	explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc) :
		healthFunc(hcf)
	{
        cout<<"GameCharacter 构造函数"<<endl;
	}

	int healthValue() const
	{
		return healthFunc(*this);
	}

    virtual void print() const
    {
        cout<<"GameCharacter::print 函数"<<endl;
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
            cout<<"EvilBadGuy 构造函数"<<endl;
		}
        virtual void print() const
        {
            cout<<"EvilBadGuy::print 函数"<<endl;
        }
};

class EyeCandyGuy : public GameCharacter
{
public:
	explicit EyeCandyGuy(HealthCalcFunc hcf = defaultHealthCalc) :
		GameCharacter(hcf)
		{
		}
};


// New flexibility:

// Health calculation function.
// Note: non-int return type.
short calcHealth(const GameCharacter&)
{
	return 256;
}

// Class for health calculation object.
struct HealthCalculator
{
	// Calculation function object.
	int operator()(const GameCharacter&) const
	{
		return 7;
	}
};


class GameLevel
{
public:
	// Health calculation member function.
	// Note: non-int return type.
	float health(const GameCharacter&) const
	{
		return 7.5f;
	}
};

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