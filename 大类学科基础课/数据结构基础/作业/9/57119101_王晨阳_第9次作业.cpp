#include<bits/stdc++.h>
using namespace std;

struct HT
{
	HT *lchild,*rchild;
	int value;
	HT(int x)
	{
		value=x;
		lchild=rchild=nullptr;
	}
}*root;

class compare
{
public:
	bool operator()(HT *a,HT *b)
	{
 	   return a->value>b->value; //Ð¡¸ù¶Ñ
	}
};

HT *createHT()
{
	priority_queue<HT*,vector<HT*>,compare> forest;
	HT *p=nullptr;
	srand(time(0));
	printf("Generating data by random...");
	for (int i=1;i<=15;i++)
	{
		int x=rand();
		p=new HT(x);
		printf("%d ",x);
		forest.push(p);
	}
	printf("\n");
	while (forest.size() > 1)
    {
        HT *node1=forest.top();
        forest.pop();
        HT *node2=forest.top();
        forest.pop();
        HT *root=new HT(node1->value+node2->value);
        root->lchild=node1;
        root->rchild=node2;
        forest.push(root);
    }
    root=forest.top();
    forest.pop();
    printf("Huffman Tree sucessfully built!\n");
    printf("Codes:\n");
    return root;
}

void encode(HT *root)
{
	static string code;
    if (root->lchild==nullptr && root->rchild==nullptr)
    	printf("%*d is encoded as %s\n",5,root->value,code.c_str());
    if (root->lchild!=nullptr)
    {
        code+="0";
        encode(root->lchild);
        code.erase(code.end()-1);
    }
    if (root->rchild!=nullptr)
    {
        code+="1";
        encode(root->rchild);
        code.erase(code.end()-1);
    }
}

int main()
{
	encode(createHT());
}
