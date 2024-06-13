#include<iostream>
#include<cstdio>
using namespace std;
const int Max=0xFFFFFFF;
const int m=11;
int d[m][m];
int earliest[m];
int latest[m];
int path[m];
void findEarliest(int k,int last)
{
	if(k==last)
		return;
	for(int i=0;i<m;i++)
	{
		if(d[k][i]<Max)
		{
			if((earliest[k]+d[k][i])>earliest[i])
				earliest[i]=earliest[k]+d[k][i];
			findEarliest(i,last);
		}
	}
}
void findLatest(int k,int first)
{
	if(k==first)
		return;
	for(int i=0;i<m;i++)
	{
		if(d[i][k]<Max)
		{
			if(latest[k]-d[i][k]<latest[i])
				latest[i]=latest[k]-d[i][k];
			findLatest(i,first);
		}
	}
}
int main()
{
	int i,j,k,n=m;
	int u,v,w;
	int first,last,count=0;
	int next;

	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
			d[i][j]=Max;
	}
	cout<<"请输入每条边的顶点及其权重"<<endl;
	for(i=0;i<m;i++)
	{
		cin>>u>>v>>w;
		d[u][v]=w;
	}
	first=1;
	last=8;
	for(i=0;i<m;i++)
	{
		earliest[i]=0;
		latest[i]=Max;
		path[i]=Max;
	}
	
	k=first;
	path[0]=k;
	count=1;
	findEarliest(k,last);
	printf("%d\n",earliest[last]);
	
	k=last;
	latest[k]=earliest[k];
	findLatest(k,first);
	printf("%d\n",latest[last]);
	
	k=first;
	while(k!=last)
	{
		for(i=0;i<m;i++)
		{
			if(d[k][i]!=Max&&(latest[i]-d[k][i]==earliest[k]))
			{
				path[count++]=i;
				k=i;
				break;
			}
		}
	}
	
	cout<<"关键路径为："<<endl; 
	for(i=0;path[i]!=last;i++)
	{
		cout<<path[i]<<"->";
	}
	cout<<path[i];

}
/*
1 2 1
1 3 1
2 4 1
2 5 1
3 5 1
3 6 1
4 7 3
4 8 2
5 8 1
6 7 2
7 8 1
*/

