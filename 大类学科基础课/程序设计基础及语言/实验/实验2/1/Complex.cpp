#include "Complex.h"

std::ostream &operator<<(std::ostream &output,const Complex &c)
{
	output<<"("<<c.r<<","<<c.i<<")";
	return output;
}
std::istream &operator>>(std::istream &input,Complex &c)
{
	char cc;
	input.ignore();
	input>>c.r;
	input.ignore();
	input>>c.i;
	input.ignore();
	return input;
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
Complex Complex::operator*(const Complex&c) const
{
	Complex temp((this->r * c.r)-(this->i * c.i),
				 (this->r * c.i)+(this->i * c.r));
	return temp;
}


Complex &Complex::operator=(const Complex&c)
{
	this->r = c.r;
	this->i = c.i;
	return *this;
}

bool Complex::operator==(const Complex&c) const
{
	return ((this->r == c.r)&&(this->i == c.i));
}

bool Complex::operator!=(const Complex&c) const
{
	return ((this->r != c.r)||(this->i != c.i));
}
