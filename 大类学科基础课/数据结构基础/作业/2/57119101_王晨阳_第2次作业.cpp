/*
	Name: 第二次作业 
	Author: 57119101王晨阳
	Date: 13/10/20 21:00 
	Description: 
		> 输入一个整数，作为新开辟数组的长度，创建该数组，并用随机数填充前一半，后一半为空；
		> 输入一个整数，作为插入位置，在该位置插入0；
		> 输入一个整数，删除该位置的数据；
		> 对数据进行转置。
*/


#include<bits/stdc++.h>
using namespace std;


typedef int ElemType;

typedef struct LNode
{   
    ElemType data;     		//数据域，保存结点的值
    struct LNode *next;     //指针域
}LNode,*LinkList;

int maxSize;


int InitList_Sq(LinkList &L)
{
	printf("input the size of sqlist:     ");
	scanf("%d",&maxSize);
	LinkList p=NULL,q=NULL;
	L=new LNode;
	L->next=NULL;                     // 建立头结点
	q=L;
	srand(time(0));
	for (int i=0;i<maxSize/2;++i)
	{
		p=new LNode;              // 生成新结点
		p->data=rand();     
		p->next=NULL; 
		q->next=p; 
		q=p;      
	}
	for (int i=maxSize/2;i<maxSize;++i)
	{
		p=new LNode;              // 生成新结点
		p->data=0;     
		p->next=NULL; 
		q->next=p; 
		q=p;      
	}
	delete p;
	delete q;
}

bool ListInsert_Sq(LinkList &L,int i,ElemType &e)
{
	LinkList p=NULL,s=NULL;
	p=L;
	int j=0;
	while (p && j<i-1) 
	{
		p=p->next;
		++j;
	}
	// i小于1或者大于表长
	if (!p || j>i-1)
		 return false;              
	s=new LNode;  // 生成新结点
	s->data=e;
	s->next=p->next;          	// 插入L中
	p->next=s;
	return true;
}

bool ListDelete_Sq(LinkList &L,int i,ElemType &e)
{
	LinkList p=NULL,q=NULL;
	p=L;
	int j=0;
	while (p->next && j<i-1)
	{
		//寻找第i-1个结点
		p=p->next;
		++j;
	}
	if (!(p->next) || j>i-1) 
		return false; 
	q=p->next;
	p->next=q->next;                                 
	e=q->data;
	delete q;
	return true;
}

void ListReverse_Sq(LinkList &L)
{
	LinkList p=NULL,q=NULL,r=NULL;
	p=L;
	q=p->next;
	p=NULL;
	r=q->next;
	while(r)
	{
		q->next=p;
		p=q;
		q=r;
		r=r->next;
	}
	q->next=p;
	p=q;
	q=r;
	L->next=p;
}

void ListDisplay_Sq(LinkList &L)
{
	LinkList p=NULL,q=NULL;
	p=L;p=p->next;
	while (p)
	{
		printf("%d\t",*p);
		p=p->next;
	}
	printf("\n");
}


int main()
{
	LinkList sl;
	
	//初始化 
	InitList_Sq(sl);
	printf("sqlist after initialize:      ");
	ListDisplay_Sq(sl);
	printf("\n");
	
	//插入 
	int pos,val,num=0;
	printf("input the position to insert: ");
	scanf("%d",&pos);
	ListInsert_Sq(sl,pos,num);
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
}
