#include <iostream>

/**
 *
 *若所有参数皆需类型转换，请为此采用 non-member 函数
  Declare non-member functions when type conversions should apply to all parameters.
 *
 *
 * **/

#if 0 
class Rational
{
public:
	Rational(int numerator = 0, int denominator = 1) :
	  n(numerator), d(denominator)
	{
	}

	int numerator() const { return n; }
	int denominator() const { return d; }

	const Rational operator*(const Rational& rhs) const
	{
		return Rational(n * rhs.numerator(), d * rhs.denominator());
	}

private:
	int n, d;
};


#else
class Rational
{
public:
	Rational(int numerator = 0, int denominator = 1) :
	  n(numerator), d(denominator)
	{
	}

	int numerator() const { return n; }
	int denominator() const { return d; }
	void show()
	{
		std::cout<<"n="<<n<<std::endl;
		std::cout<<"d="<<d<<std::endl;
	}
private:
	int n, d;
};


const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}


#endif

int main()
{
	Rational oneEighth(1, 8);
	Rational oneFourth(1, 4);

	Rational result;
	result = oneFourth * oneEighth;		// fine
	result = result * oneEighth;		// fine

	result = oneFourth * 2;				// fine
	result = 2 * oneFourth;				// hooray, it works!

	result.show();
	return 0;
}
