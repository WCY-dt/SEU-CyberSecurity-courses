#include<bits/stdc++.h>
using namespace std;

const int Maxsize=1e3+1;

int m[Maxsize][Maxsize];

typedef struct
{
	int x,y;
	int e;
}Triple;

typedef struct
{
	Triple data[Maxsize];
	int rpos[Maxsize];
	int mu,nu,tu;
}RLSMatrix;

void initialMatrix(int matrixSize,RLSMatrix &rlsMatrix)
{
	rlsMatrix.mu=rlsMatrix.nu=matrixSize;
	rlsMatrix.tu=0;
	for (int i=1;i<=rlsMatrix.mu;i++)
	{
		rlsMatrix.rpos[i]=rlsMatrix.tu+1;
		for (int j=1;j<=rlsMatrix.nu;j++)
		{
			if (rand()%20==0)
			{
				rlsMatrix.tu++;
				m[i][j]=rand()%100+1;
				rlsMatrix.data[rlsMatrix.tu].x=i;
				rlsMatrix.data[rlsMatrix.tu].y=j;
				rlsMatrix.data[rlsMatrix.tu].e=m[i][j];
			}
			else m[i][j]=0;
		}
	}
}

void matrixMultiply(RLSMatrix m1,RLSMatrix m2,RLSMatrix &ans)
{
	int colTemp[Maxsize];
	ans.mu=m1.mu;
	ans.nu=m2.nu;
	ans.tu=0;
	for (int ansRow=1;ansRow<=m1.mu;ansRow++) 
	{
		memset(colTemp,0,sizeof(colTemp));
		ans.rpos[ansRow]=ans.tu;
		int m1temp=(ansRow<m1.mu?m1.rpos[ansRow+1]-1:m1.tu);
		for (int rpos1=m1.rpos[ansRow];rpos1<=m1temp;rpos1++) 
		{
			int m2Row=m1.data[rpos1].y;
			int m2temp=(m2Row<m2.mu?m2.rpos[m2Row+1]-1:m2.tu);
			for (int rpos2=m2.rpos[m2Row];rpos2<=m2temp;rpos2++)
				colTemp[m2.data[rpos2].y]+=(m1.data[rpos1].e*m2.data[rpos2].e);
		}
		for (int ansCol=1;ansCol<=ans.nu;ansCol++)
			if (colTemp[ansCol]>0) 
			{
				ans.tu++;
				ans.data[ans.tu].x=ansRow;
				ans.data[ans.tu].y=ansCol;
				ans.data[ans.tu].e=colTemp[ansCol];
			}
	}
}

void printMatrix(RLSMatrix m)
{
	for (int i=1;i<=m.tu;i++)
		printf("%d %d %d\n",m.data[i].x,m.data[i].y,m.data[i].e);
}

int main()
{
	srand(time(0));
	int n;
	printf("矩阵维数为: "); 
	scanf("%d",&n);
	RLSMatrix A,B,C;

	initialMatrix(n,A);
	printf("\n矩阵A为:\n");
	printMatrix(A);
	initialMatrix(n,B);
	printf("\n矩阵B为:\n");
	printMatrix(B);
	
	matrixMultiply(A,B,C);
	printf("\nA*B的结果为:\n");
	if (C.tu==0) printf("0矩阵");
	else printMatrix(C);
	
	return 0; 
}

