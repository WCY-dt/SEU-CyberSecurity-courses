#include "Complex.h"
using std::cin;
using std::cout;

int main()
{
	Complex x,y(4.3,8.2),z(3.3,1.1),k;
	cout<<"Enter a complex number in the form (a,b): ";
	cin>>k;
	cout<<"x: "<<x<<"\ny: "<<y<<"\nz: "<<z<<"\nk: "<<k<<'\n';
	x=y+z;
	cout<<"\nx=y+z:\n"<<x<<"="<<y<<"+"<<z<<'\n';
	x=y-z;
	cout<<"\nx=y-z:\n"<<x<<"="<<y<<"-"<<z<<'\n';
	x=y*z;
 	cout<<"\nx=y*z:\n"<<x<<"="<<y<<"*"<<z<<"\n\n";
	if (x!=k)
		cout<<x<<"!="<<k<<'\n';
	cout<<'\n';
	x=k;
	if (x==k)
		cout<<x<<"=="<<k<<'\n';
	return 0;
}
