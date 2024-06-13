#include<algorithm>
#include<cstdlib>
#include<time.h>


/*------------------------constant definition------------------------*/

typedef int elemType;
const int numOfEle=25;


/*--------------basic structure & function definition--------------*/

struct node
{
	elemType e;
	node* l;
	node* r;
	int h;
};

int getH(node*& Node)
{
	if (Node==nullptr)
		return -1;
	else
		return Node->h;
}
 
node* fMax(node*& Node)//[find maximum]
{
	if (!Node)
		while (Node->r!=nullptr)
			Node=Node->r;
	return Node;
}
 
node* fMin(node*& Node)//[find minimum]
{
	if (!Node)
		while (Node->l!=nullptr)
			Node=Node->l;
	return Node;
}


/*----------------auxiliary functions for algorithm----------------*/

node* fEle(node*& Node,elemType x)//[find element]
{
	if (!Node) return nullptr;
	
	if (Node->e==x) return Node;
	
	if (Node->e>x)
		Node->l=fEle(Node->l,x);
	else if (Node->e<x)
		Node->r=fEle(Node->r,x);
}

void RR(node*& Node)
{
	if (!Node) return;
	
	auto ptr=Node->l;
	Node->l=ptr->r;
	ptr->r=Node;
	Node=ptr;
	Node->h=std::max(getH(Node->l),getH(Node->r))+1;
	ptr->h=std::max(getH(ptr->l),Node->h)+1;
}
 
void LL(node*& Node)
{
	if (!Node) return;
	
	auto ptr=Node->r;
	Node->r=ptr->l;
	ptr->l=Node;
	Node=ptr;
	Node->h=std::max(getH(Node->l),getH(Node->r))+1;
	ptr->h=std::max(getH(ptr->r),Node->h)+1;
}
 
void RL(node*& Node)
{
	if (!Node) return;
	
	RR(Node->r);
	LL(Node);
}
 
void LR(node*& Node)
{
	if (!Node) return;
	
	LL(Node->l);
	RR(Node);
}

void print(node*& Node)
{
	if (!Node) return;
	
	printf("%-3d ",Node->e);
	print(Node->l);
	print(Node->r);
}


/*-------------------algorithm implementation-------------------*/

void Insert(node*& Node,elemType x)
{
	int rh=0,lh=0;
	
	if (!Node)
	{
		Node=new node;
		Node->e=x;
		Node->h=0;
		Node->l=Node->r=nullptr;
	}
	else
	{
		if (Node->e>x)
		{
			Insert(Node->l,x);
			
			rh=getH(Node->r);
			lh=getH(Node->l);
			
			if ((lh-rh)>1)
			{
				if (Node->l->e>x)
					RR(Node);
				else
					LR(Node);
			}
		}
		
		else if (Node->e<x)
		{
			Insert(Node->r,x);
			
			rh=getH(Node->r);
			lh=getH(Node->l);
			
			if ((rh-lh)>1)
			{
				if (Node->r->e<x)
					LL(Node);
				else
					RL(Node);
			}
		}
	}
 
	Node->h=std::max(getH(Node->l),getH(Node->r))+1;
}
 
void Delete(node*& Node,elemType x)
{
	int rh=0,lh=0,rhh=0,lhh=0;
	node* temp=nullptr;
	
	if (!Node) return;
	
	else if (Node->e>x)
	{
		Delete(Node->l,x);
		
		rh=getH(Node->r);
		lh=getH(Node->l);
		
		if ((rh-lh)>1)
		{
			rhh=getH(Node->r->r);
			lhh=getH(Node->r->l);
			
			if (rhh>lhh)
				LL(Node);
			else
				RL(Node);
		}
	}
	
	else if (Node->e<x)
	{
		Delete(Node->r,x);
		rh=getH(Node->r);
		lh=getH(Node->l);
		if ((lh-rh)>1)
		{
			rhh=getH(Node->l->r);
			lhh=getH(Node->l->l);
 
			if (rhh<lhh)
				RR(Node);
			else
				LR(Node);
		}
	}
	else
	{
		temp=Node;
		
		if (Node->l!=nullptr && Node->r!=nullptr)
		{
			temp=fMin(Node->r);
			Node->e=temp->e;
			Delete(Node->r,Node->e);
		}
		
		else if (Node->l==nullptr)
			Node=Node->r;
		else if (Node->r==nullptr)
			Node=Node->l;
			
		else
			delete temp;
	}
}


/*--------------------------main function--------------------------*/

int main()
{
	//generate elements randomly
	std::vector<int>vec;
	std::srand(time(0));
	for (int i=1;i<=numOfEle;i++)
		vec.push_back(std::rand()%100);
	node* Node=nullptr;
	for (int i=0;i<numOfEle;i++)
	{
		printf("Inserting element %2d: %-2d ...\n",i+1,vec[i]);
		Insert(Node,vec[i]);
	}
	
	//preorder traversal
	printf("\npreorder traversal:\n");
	print(Node);
	printf("\n\n");
	
	//delete elements
	for (int i=0;i<numOfEle-1;i++)
	{
		printf("Deleting element %2d: %-2d ...\n",i+1,vec[i]);
		Delete(Node,vec[i]);
	}
} 
