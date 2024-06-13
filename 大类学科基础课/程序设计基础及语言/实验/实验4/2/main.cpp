#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
using std::string;

class shape
{
public:
	shape(){};
	~shape(){};

	double x;
	double y;
	string name;
	virtual void print() {}
};

class circle : public shape
{
public:
	circle(string n = "none", double ix = 0, double iy = 0, double r = 0) : radius(r)
	{
		x = ix;
		y = iy;
		name = n;
	}

	~circle() {}

	double area()
	{
		return 3.14 * radius * radius;
	}

	void print()
	{
		cout << name << " " << area() << endl;
	}

protected:
	double radius;
};

class triangle : public shape
{
public:
	triangle(string n = "noname", double ix = 0, double iy = 0, double il1 = 0, double il2 = 0, double il3 = 0) : l1(il1), l2(il2), l3(il3)
	{
		x = ix;
		y = iy;
		name = n;
	}

	double area()
	{
		double p = (l1 + l2 + l3) / 2;
		return sqrt(p * (p - l1) * (p - l2) * (p - l3));
	}

	~triangle() {}

	void print()
	{
		cout << name << " " << area() << endl;
	}

private:
	double l1;
	double l2;
	double l3;
};

class rectangle : public shape
{
public:
	rectangle(string n = "none", double ix = 0, double iy = 0, double il1 = 0, double il2 = 0) : l1(il1), l2(il2)
	{
		x = ix;
		y = iy;
		name = n;
	}

	double area()
	{
		return l1 * l2;
	}

	~rectangle() {}

	void print()
	{
		cout << name << " " << area() << endl;
	}

protected:
	double l1;
	double l2;
};

class cube : public shape
{
public:
	cube(string iname = "none", double x = 0, double y = 0, double z = 0)
	{
		name = iname;
		volume = x * y * z;
	}

	~cube() {}

	void print()
	{
		cout << name << " " << volume << endl;
	}

protected:
	double volume;
};

class square : public rectangle
{
public:
	square(string iname = "none", double ix = 0, double iy = 0, double il1 = 0, double il2 = 0)
	{
		x = ix;
		y = iy;
		name = iname;
		l1 = il1;
		l2 = il2;
	}

	~square() {}

	double area()
	{
		return rectangle::area();
	}

	void print()
	{
		rectangle::print();
	}
};

int main()
{
	shape *shapes[5];
	shapes[0] = new circle("circle", 1, 1, 3);
	shapes[1] = new triangle("triangle", 1, 1, 3, 4, 5);
	shapes[2] = new rectangle("rectangle", 1, 1, 3, 4);
	shapes[3] = new square("square", 1, 1, 3, 3);
	shapes[4] = new cube("cube", 3, 4, 5);
	for (int i = 0; i < 5; i++)
	{
		shapes[i]->print();
	}
}