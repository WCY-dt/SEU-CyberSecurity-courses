/*
	Name: 第三作业 
	Author: 57119101王晨阳
	Date: 21/10/20 19:00 
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
	Coord(int X,int Y)//初始化 
	{
		this->x=X;
		this->y=Y;
	}
	int x;
	int y;
};


bool dfs(int a,int b,stack<Coord> &s)//基于stack的记录路径的深搜 
{
	if (Map[a][b]==1) return false;//碰到墙或者已经走过的地方 
	Map[a][b]=1;
	Coord c(a,b);
	s.push(c);
	if (a==maxRow-2 && b==maxColume-2)//到终点了 
		return true;
	bool finded;
	//搜索四个方向，如果其中一个找到了，就不再搜索 
	finded=dfs(a+1,b,s);
	if (!finded) finded=dfs(a,b+1,s);
	if (!finded) finded=dfs(a-1,b,s);
	if (!finded) finded=dfs(a,b-1,s);
	if (!finded)
	{
		s.pop();
		return false;
	}
	return true;
}


void Output(stack<Coord> &s)//输出 
{
	stack <Coord> op;
	//stack pop出的路径是反过来的，用另一个stack把它正过来 
	while (!s.empty())
	{
		op.push(s.top());
		s.pop();
	}
	printf("找到路啦! \n路径为 ( %d , %d )\n",op.top().x,op.top().y);
	op.pop();
	while (!op.empty())
	{
		printf("    -> ( %d , %d )\n",op.top().x,op.top().y);
		op.pop();
	}
}


int main()
{
	bool finded;
	stack <Coord> s;
	finded=dfs(1,1,s);
	if (!finded) printf("哎呀，没找到路径~~(哭)");
	else Output(s);
} 
