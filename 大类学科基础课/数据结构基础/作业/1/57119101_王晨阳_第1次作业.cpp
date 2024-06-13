/*
	Name: 第一次作业 
	Author: 57119101王晨阳
	Date: 09/10/20 20:00 
	Description: 
		> 输入一个整数，作为新开辟数组的长度，创建该数组，并用随机数填充前一半，后一半为空；
		> 输入一个整数，作为插入位置，在该位置插入0；
		> 输入一个整数，删除该位置的数据；
		> 对数据进行转置。
*/

#include<bits/stdc++.h>
using namespace std;


typedef int ElemType;
typedef struct 
{
	ElemType *elem;
	int length;								//当前表内容长度 
	int listsize;							//可使用总容量
}SqList;


int InitList_Sq(SqList &L) 
{
	int maxSize;
	printf("input the size of sqlist:     ");
	scanf("%d",&maxSize);
	L.elem=new ElemType[maxSize];
	L.length=maxSize/2; 					//空表当前长度为0
	L.listsize=maxSize; 					//初始存储容量
	srand(time(0));
	for (int i=0;i<L.length;i++)
		L.elem[i]=rand();
}

bool ListInsert_Sq(SqList &L,int i,ElemType e)
{
	ElemType *p;
	if (i<1 || i>L.length+1) return false;	//i的合法值为1≤i≤L.length+1  
	if (L.length>=L.listsize) return false;	//队列是不是已经满了
	ElemType *q=&(L.elem[i-1]);
	for (p=&(L.elem[L.length-1]);p>=q;--p)
        *(p+1)=*p;
	*q=e; 
	++L.length; 
	return true;
}

bool ListDelete_Sq(SqList &L,int i,ElemType &e)
{
	ElemType *p,*q;
	if (i<1 || i>L.length) return false; 	//i的合法值为1≤i≤L.length
	p=&(L.elem[i-1]);
	e=*p;
	q=L.elem+L.length-1;
	for (++p;p<=q;++p)
		*(p-1)=*p;  						//被删除元素之后的元素左移
	--L.length;
	return true;
}

void ListReverse_Sq(SqList &L)
{
	ElemType *p,*q;
	p=&(L.elem[0]);
	q=&(L.elem[L.length-1]);
	while (p<q)
	{
		swap(*p,*q);
		++p;
		--q;
	}
}

void ListDisplay_Sq(SqList &L)
{
	ElemType *p;
	for (p=&(L.elem[0]);p<=&(L.elem[L.length-1]);++p)
		printf("%d\t",*p);
	printf("\n");
}


int main()
{
	SqList sl;
	
	//初始化 
	InitList_Sq(sl);
	printf("sqlist after initialize:      ");
	ListDisplay_Sq(sl);
	printf("\n");
	
	//插入 
	int pos,val;
	printf("input the position to insert: ");
	scanf("%d",&pos);
	ListInsert_Sq(sl,pos,0);
	printf("sqlist after insert:          ");
	ListDisplay_Sq(sl);
	printf("\n");
	
	//删除 
	int deletedElem;					//被删除的元素 
	printf("input the position to delete: ");
	scanf("%d",&pos);
	ListDelete_Sq(sl,2,deletedElem);
	printf("sqlist after delete:          ");
	ListDisplay_Sq(sl);
	printf("the element deleted is:       %d\n",deletedElem);
	printf("\n");
	
	//转置 
	ListReverse_Sq(sl);
	printf("sqlist after reverse:         ");
	ListDisplay_Sq(sl);
	printf("\n");
	
	delete []sl.elem;
}
