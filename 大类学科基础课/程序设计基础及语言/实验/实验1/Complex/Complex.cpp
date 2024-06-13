#include "Complex.h"

ostream &operator<<(ostream &output,const Complex &c)
{
	output<<"("<<c.r<<","<<c.i<<")";
	return output;
}
	
Complex::Complex(double real,double imaginary)
{
	this->r=real;
	this->i=imaginary;
}

Complex Complex::operator+(const Complex&c) const
{
	Complex temp(this->r + c.r,this->i + c.i);
	return temp;
}
Complex Complex::operator-(const Complex&c) const
{
	Complex temp(this->r - c.r,this->i - c.i);
	return temp;
}
