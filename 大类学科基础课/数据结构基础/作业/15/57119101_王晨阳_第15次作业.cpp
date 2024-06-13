#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<queue>
#include<string>
#include<iostream>

using eleType=int;
using infoType=char;

struct HT
{
	HT *lchild,*rchild;
	infoType chara;
	eleType value;
	HT(eleType x,infoType c)
	{
		value=x;
		chara=c;
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

void count(std::string str,int *ascii)
{
	char tmp;
	for (int i=0;i<=127;i++)
		ascii[i]=0;
	for (int i=0;i<str.length();i++)
		ascii[str[i]]++;
}

HT *createHT(int *ascii)
{
	std::priority_queue<HT*,std::vector<HT*>,compare> forest;
	HT *p=nullptr;
	for (int i=0;i<=127;i++)
	{
		if (ascii[i]!=0)
		{
			p=new HT(ascii[i],(char)i);
			forest.push(p);
		}
	}
	while (forest.size() > 1)
    {
        HT *node1=forest.top();
        forest.pop();
        HT *node2=forest.top();
        forest.pop();
        HT *root=new HT(node1->value+node2->value,0);
        root->lchild=node1;
        root->rchild=node2;
        forest.push(root);
    }
    root=forest.top();
    forest.pop();
    return root;
}

void encode(HT *root,int *ascii)
{
	static std::string code;
    if (root->lchild==nullptr && root->rchild==nullptr)
    	printf("%c is encoded as %s\n",root->chara,code.c_str());
    if (root->lchild!=nullptr)
    {
        code+="0";
        encode(root->lchild,ascii);
        code.erase(code.end()-1);
    }
    if (root->rchild!=nullptr)
    {
        code+="1";
        encode(root->rchild,ascii);
        code.erase(code.end()-1);
    }
}

int main()
{
	int ascii[128];
	std::string str;
	getline(std::cin,str);
	count(str,ascii);
	encode(createHT(ascii),ascii);
}
