#include "RationalNumber.h"
#include<iostream>

RationalNumber::RationalNumber(int numerator,int denominator)
{
	n=numerator;
	if (denominator!=0) d=denominator;
	else d=1;
	this->reduction();
}
	
RationalNumber RationalNumber::operator+(const RationalNumber& r)
{
	RationalNumber temp(this->n * r.d + this->d * r.n,this->d * r.d);
	temp.reduction();
	return temp;
}
RationalNumber RationalNumber::operator-(const RationalNumber& r)
{
	RationalNumber temp(this->n * r.d - this->d * r.n,this->d * r.d);
	temp.reduction();
	return temp;
}
RationalNumber RationalNumber::operator*(const RationalNumber& r)
{
	RationalNumber temp(this->n * r.n,this->d * r.d);
	temp.reduction();
	return temp;
}
RationalNumber RationalNumber::operator/(	   RationalNumber& r)
{
	if (r.d==0)
	{
		std::cout<<"WRONG!";
	}
	else
	{
		RationalNumber temp(this->n * r.d,this->d * r.n);
		temp.reduction();
		return temp;
	}
}

bool RationalNumber::operator> (const RationalNumber& r) const
{
	return (this->n * r.d - this->d * r.n > 0);
}
bool RationalNumber::operator< (const RationalNumber& r) const
{
	return (this->n * r.d - this->d * r.n < 0);
}
bool RationalNumber::operator>=(const RationalNumber& r) const
{
	return (this->n * r.d - this->d * r.n >=0);
}
bool RationalNumber::operator<=(const RationalNumber& r) const
{
	return (this->n * r.d - this->d * r.n >=0);
}

bool RationalNumber::operator==(const RationalNumber& r) const
{
	return (this->n * r.d == this->d * r.n);
}
bool RationalNumber::operator!=(const RationalNumber& r) const
{
	return (this->n * r.d == this->d * r.n);
}

void RationalNumber::printRational() const
{
	if (d==1) std::cout<< this->n ;
	else std::cout<< this->n <<'/'<< this->d ;
}
	
void RationalNumber::reduction()
{
	for (int i=2;i<=std::min(n,d);i++)
		if (n%i==0 && d%i==0)
		{
			n/=i;
			d/=i;
		}
}
