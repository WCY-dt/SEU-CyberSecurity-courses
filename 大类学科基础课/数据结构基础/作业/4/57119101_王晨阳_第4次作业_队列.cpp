/*
	Name: 第三作业 
	Author: 57119101王晨阳
	Date: 28/10/20 19:13
	Description: 
		> 使用栈及队列分别设计一个算法进行迷宫路径查找（用下列二维数组表示迷宫，
		  1表示不可通过，0表示可以通过，左上角2为起点，右下角3为终点）
*/


#include<bits/stdc++.h>
using namespace std;


#define maxRow 15//Map行数 
#define maxColume 15//Map列数 


int Map[maxRow][maxColume] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            				   { 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            				   { 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1 },
            				   { 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
            				   { 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
            				   { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
            				   { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
            				   { 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1 },
            				   { 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1 },
            				   { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1 },
            				   { 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1 },
            				   { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1 },
            				   { 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 },
            				   { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1 },
            				   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }};
            				   
            				   
struct Coord//记录坐标 
{
	Coord(int X=0,int Y=0)//初始化 
	{
		this->x=X;
		this->y=Y;
	}
	int x;
	int y;
};

Coord pre[maxRow][maxColume];

void bfs(queue<Coord> &q)//基于queue的记录路径的宽搜 
{
	q.push(Coord(1,1));
	while (!q.empty())
	{
		Coord c(q.front().x,q.front().y);
		if (c.x==maxRow-2 && c.y==maxColume-2)
		{
			return;
		}
		q.pop();
		if (Map[c.x+1][c.y]!=1) Map[c.x+1][c.y]=1,q.push(Coord(c.x+1,c.y)),pre[c.x+1][c.y]=Coord(c.x,c.y);
		if (Map[c.x-1][c.y]!=1) Map[c.x-1][c.y]=1,q.push(Coord(c.x-1,c.y)),pre[c.x-1][c.y]=Coord(c.x,c.y);
		if (Map[c.x][c.y+1]!=1) Map[c.x][c.y+1]=1,q.push(Coord(c.x,c.y+1)),pre[c.x][c.y+1]=Coord(c.x,c.y);
		if (Map[c.x][c.y-1]!=1) Map[c.x][c.y-1]=1,q.push(Coord(c.x,c.y-1)),pre[c.x][c.y-1]=Coord(c.x,c.y);
	}
}


void Output()//输出 
{
	stack <Coord> s;
	Coord c(maxRow-2,maxColume-2);
	s.push(c);
	while (!(c.x==1 && c.y==1))
	{
		c=pre[c.x][c.y];
		s.push(c);
	}
	printf("找到路啦! \n路径为 ( %d , %d )\n",s.top().x,s.top().y);
	s.pop();
	while (!s.empty())
	{
		printf("    -> ( %d , %d )\n",s.top().x,s.top().y);
		s.pop();
	}
}


int main()
{
	queue <Coord> q;
	bfs(q);
	if (Map[maxRow-2][maxColume-2]!=1) printf("哎呀，没找到路径~~(哭)");
	else Output();
} 
