#include<iostream>

using std::cout;
using std::endl; 

class SimpleCalculator
{
public:
	double add(double a,double b)
	{
        return a+b;
	}
	double sub(double a,double b)
	{
        return a-b;
	}
	double mul(double a,double b)
	{
        return a*b;
	}
	double div(double a,double b)
	{
		return a/b;
	}
};

int main()
{
    double a=10;
    double b=20;
    SimpleCalculator sc;
	cout<<"The value of a is: " <<a<<endl;
	cout<<"The value of b is: " <<b<<endl;

	cout<<"Adding a and b yields "<<sc.add(a,b)<<endl;
	cout<<"subtracting b from a yields "<<sc.sub(a,b)<<endl;
	cout<<"multiplying a by b yields "<<sc.mul(a,b)<<endl;
	cout<<"dividing a by b yields "<<sc.div(a,b)<<endl;
}
