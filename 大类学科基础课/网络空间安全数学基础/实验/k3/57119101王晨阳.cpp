#include<bits/stdc++.h>
using namespace std;

const int numberOfR=3;

const int r[numberOfR]={1,0,1};

const int G[numberOfR][13]={{10,12,1,5,2,6,13,3,8,9,7,11,4},
				  		    {3,6,7,1,10,13,4,2,11,12,9,5,8},
				            {4,1,3,13,11,10,6,12,8,9,5,7,2}};

int Ans[13]={1,2,3,4,5,6,7,8,9,10,11,12,13};

char c[13]={'A','2','3','4','5','6','7','8','9','10','J','Q','K'};

void tranverse(int n)
{
	int tmp[13];
	for (int i=0;i<13;i++)
		tmp[G[n][i]-1]=Ans[i];
	for (int i=0;i<13;i++)
		Ans[i]=tmp[i];
}

int main()
{
	for (int i=0;i<numberOfR;i++)
	{
		for (int j=1;j<=r[i];j++)
		{
			tranverse(i);
		}
	}
	for (int i=0;i<13;i++)
	{
		printf("%c ",c[Ans[i]-1]);
	}
}
