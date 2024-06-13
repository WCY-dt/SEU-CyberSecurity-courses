#include "RationalNumber.h"
#include<iostream>
using std::cout;

int main()
{
	RationalNumber r1(7,3);
	RationalNumber r2(1,3);
	RationalNumber r3;
	r3=r1+r2;
	r1.printRational();
	cout<<" + ";
	r2.printRational();
	cout<<" = ";
	r3.printRational();
	cout<<'\n';
	
	r3=r1-r2;
	r1.printRational();
	cout<<" - ";
	r2.printRational();
	cout<<" = ";
	r3.printRational();
	cout<<'\n';
	
	r3=r1*r2;
	r1.printRational();
	cout<<" * ";
	r2.printRational();
	cout<<" = ";
	r3.printRational();
	cout<<'\n';
	
	r3=r1/r2;
	r1.printRational();
	cout<<" / ";
	r2.printRational();
	cout<<" = ";
	r3.printRational();
	cout<<'\n';
	
	r1.printRational();
	cout<<((r1<r2)?" < ":" > ");
	r2.printRational();
	cout<<'\n';
	
	r1.printRational();
	cout<<((r1==r2)?" = ":" != ");
	r2.printRational();
	cout<<'\n';
}
