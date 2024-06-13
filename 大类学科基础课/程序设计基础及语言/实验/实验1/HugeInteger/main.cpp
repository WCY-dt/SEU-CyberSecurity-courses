#include "HugeInteger.h"

#include<iostream>
using std::cin;
using std::cout;

int main()
{
	char h1[100],h2[100],h3[100];
	
	cout<<"n1 is ";
	cin.getline(h1,99);
	cout<<"n2 is ";
	cin.getline(h2,99);
	cout<<"n3 is ";
	cin.getline(h3,99);
	
	HugeInteger n1(h1),n2(h2),n3(h3);
	
	if (n1.isEqualTo(n2))
		n1.output(),std::cout<<" is equal to ",n2.output(),std::cout<<std::endl;
	if (n1.isNotEqualTo(n2))
		n1.output(),std::cout<<" is not equal to ",n2.output(),std::cout<<std::endl;
	
	if (n1.isLessThan(n2))
		n1.output(),std::cout<<" is less than ",n2.output(),std::cout<<std::endl;
	if (n1.isGreaterThan(n2))
		n1.output(),std::cout<<" is more than ",n2.output(),std::cout<<std::endl;
	
	if (n1.isLessThanOrEqualTo(n2))
		n1.output(),std::cout<<" is less than or equal to ",n2.output(),std::cout<<std::endl;
	if (n1.isGreaterThanOrEqualTo(n2))
		n1.output(),std::cout<<" is more than or equal to ",n2.output(),std::cout<<std::endl;
	
	n1.output();
	std::cout<<" + ";
	n2.output();
	std::cout<<" = ";
	(n1.add(n2)).output();
	std::cout<<"\n\n";
	
	n1.output();
	std::cout<<" - ";
	n2.output();
	std::cout<<" = ";
	(n1.substract(n2)).output();
	std::cout<<"\n\n";
	
	if (n3.isZero())
		std::cout<<"n3 contains value 0";
}
/*
7654321
765
0
*/
