#include "Hugeint.h"
using std::cin;
using std::cout;

int main()

{
	char h1[100],h2[100],h3[100],h4[100],h5[100],h6[100];
	
	cout<<"n1 is ";
	cin.getline(h1,99);
	cout<<"n2 is ";
	cin.getline(h2,99);
	cout<<"n3 is ";
	cin.getline(h3,99);
	cout<<"n4 is ";
	cin.getline(h4,99);
	cout<<"n5 is ";
	cin.getline(h5,99);
	cout<<"n6 is ";
	cin.getline(h6,99);
	
	HugeInt n1(h1),n2(h2),n3(h3),n4(h4),n5(h5),n6(h6);
	
	if (n1==n2)
		cout<<"n1 is equal to n2\n";
	if (n1!=n2)
		cout<<"n1 is not equal to n2\n";
	
	if (n1<n2)
		cout<<"n1 is less than n2\n";
	if (n1>n2)
		cout<<"n1 is more than n2\n";
	
	if (n1<=n2)
		cout<<"n1 is less than or equal to n2\n";
	if (n1>=n2)
		cout<<"n1 is more than or equal to n2\n";
	
	cout<<n1<<"+"<<n2<<"="<<n1+n2<<"\n\n";
	cout<<n3<<"+"<<n4<<"="<<n3+n4<<"\n\n";
	
	cout<<n5<<"-"<<n6<<"="<<n5-n6<<"\n";
	cout<<n5<<"*"<<n6<<"="<<n5*n6<<"\n";
	cout<<n5<<"/"<<n6<<"="<<n5/n6<<"\n";
}
/*
7654321
7891234
99999999999999999999999999999
1
12341234
7888

*/
