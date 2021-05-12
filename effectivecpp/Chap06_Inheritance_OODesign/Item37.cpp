#include <iostream>
using namespace std;


/**
 * 条款37：绝不重新定义继承而来的缺省参数值
	Never redefine a function's inherited default parameter value.

	virtual 函数是动态绑定，而缺省参数值却是静态绑定。
	如果重新定义了继承而来的缺省参数值，可能会在”调用一个定义于 derived class 内的 virtual 函数“时，使用 base class 为它所指定的缺省参数值。
 * 
 * 
 * **/

class Shape
{
public:
	enum ShapeColor { Red, Green, Blue };

	// Non non-virtual calls virtual function
	void draw(ShapeColor color = Green) const
	{
		cout<<"Shape::draw: "<<color<<endl;
		doDraw(color);
	}

private:
	// Private pure virtual function does actual work.
	virtual void doDraw(ShapeColor color=Red) const = 0;
};


class Rectangle : public Shape
{
private:
	// Notice lack of default parameter value
	virtual void doDraw(ShapeColor color=Blue) const
	{
		cout<<"Rectangle::doDraw: "<<color<<endl;
	}
};


class Circle : public Shape
{
	// Don't redefine inherited default parameter value!
	virtual void doDraw(ShapeColor color) const
	{
		cout<<"Circle::doDraw"<<endl;
	}
};

int main()
{
	Shape* pr = new Rectangle;	// static type = Shape*		dynaminc type = Rectangle*
	Shape* pc = new Circle;		// static type = Shape*		dynaminc type = Circle*


	
	// Calls Shape::draw(Shape::Blue);
	// Calls Rectangle::doDraw(Shape::Blue);
	pr->draw(Shape::Blue);

	// Calls Shape::draw();
	// Calls Rectangle::doDraw(Shape::Red);
	pr->draw();


	// Calls Shape::draw(Shape::Red);
	// Calls Circle::draw(Shape::Red);
	//pc->draw(Shape::Red);

	// Calls Shape::draw();
	// Calls Circle::draw(Shape::Red);
	//pc->draw();


	delete pc;
	delete pr;

	return 0;
}