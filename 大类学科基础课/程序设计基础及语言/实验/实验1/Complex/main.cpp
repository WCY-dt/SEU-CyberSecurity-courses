#include "Complex.h"
using std::cin;
using std::cout;

int main()
{
	Complex x(1,7),y(9,2),z(10,1),t(11,5);
	cout<<x<<'+'<<y<<'='<<x+y<<'\n';
	cout<<z<<'-'<<t<<'='<<z-t<<'\n';
	return 0;
}
