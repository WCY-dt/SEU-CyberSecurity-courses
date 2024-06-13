#include<bits/stdc++.h>
using namespace std;
const int n=15;
int h[16];

void up(int temp)
{
	int x=temp;
	while (x>1 && h[x]>h[x/2]) 
	{
		swap(h[x],h[x/2]);
		x/=2;
	}
}

void init()
{
	srand(time(0));
	for (int i=1;i<=n;i++)
	{
		h[i]=rand();
		printf("%d ",h[i]);
		up(i);
	}
	printf("\n");
}

void print(int x)
{
	printf("%d ",h[x]);
	if (x*2<=n) print(x*2);
	if (x*2+1<=n) print(x*2+1);
}

int main()
{
	init();
	print(1);
}
