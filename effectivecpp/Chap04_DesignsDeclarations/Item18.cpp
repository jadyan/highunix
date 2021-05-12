#include<iostream>
#if 0
struct Day
{
explicit Day(int d) : val(d) {}
int val;
};

struct Month
{
explicit Month(int m) : val(m) {}
int val;
};

struct Year
{
explicit Year(int y) : val(y) {}
int val;
};


class Date
{
 public:
	Date(const Month& m, const Day& d, const Year& y) :
	   month(m.val), day(d.val), year(y.val)
	{
	}

private:
	int month, day, year;
};

#endif

/*
 *
 *条款18：让接口容易被正确使用，不易被误用
		  Make interfaces easy to use correctly and hard to use incorrectly.
		  
 *
 * */

struct Day
{
explicit Day(int d) : val(d) {}
int val;
};

class Month
{
public:
	static Month Jan() { return Month(1); }
	static Month Feb() { return Month(2); }
	static Month Mar() { return Month(3); }
	static Month apr() { return Month(4); }
	static Month May() { return Month(5); }
	static Month Jun() { return Month(6); }
	static Month Jul() { return Month(7); }
	static Month Aug() { return Month(8); }
	static Month Sep() { return Month(9); }
	static Month Oct() { return Month(10); }
	static Month Nov() { return Month(11); }
	static Month Dec() { return Month(12); }

	Month() {}
	int getMOnthNum() const
	{
		return val;
	}

private:
	explicit Month(int m) : val(m)
	{
	}

	int val;
};

struct Year
{
	explicit Year(int y) : val(y) {}
	int val;
};


class Date2
{
 public:
	Date2(const Month& m, const Day& d, const Year& y) :
	   month(m), day(d.val), year(y.val)
	{
	}
	void print()
	{
		std::cout<<"year:"<<year<<std::endl;
		std::cout<<"MOnth:"<<month.getMOnthNum()<<std::endl;
		std::cout<<"Day:"<<day<<std::endl;
		
	}
private:
	int day, year;
	Month month;
};







int main(int argc , char ** argv)
{
	Date2  date(Month::Feb(), Day(27), Year(1988));
	date.print();
}
