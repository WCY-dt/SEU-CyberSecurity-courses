#include <bits/stdc++.h>
using namespace std;

struct Node
{
    bool ltag,rtag;
    int data;
    Node *lchild,*rchild;
    Node()
    {
        ltag=rtag=false;
        data=-1;
        lchild=rchild=nullptr;
    }
};

class TBT
{
public:
    TBT();
    void addHeader();                       // 构环
    void print();                           // 中序遍历
private:
    Node *create();
    void threaded(Node *cur, Node *& pre);  // 线索化
    Node *getSuc(Node *node);               // 返回后继
    Node *getPre(Node *node);               // 返回前驱
    Node *header;  							// 头结点
    Node *root;                             // 根结点
};

TBT::TBT()
{
    header=new Node;
    root=create();
}

Node *TBT::create()
{
    Node *p=nullptr;
    int x;
    scanf("%d",&x);
    if (x==-1) p=nullptr;
    else
    {
        p=new Node;
        p->data=x;
        p->lchild=create();
        p->rchild=create();
    }
    return p;
}

void TBT::threaded(Node *cur, Node *& pre)
{
    if (cur==nullptr) return;
    else
    {
        threaded(cur->lchild,pre);//左子树 
        if (cur->lchild==nullptr)//当前节点 
        {
            cur->ltag=true;
            cur->lchild=pre;
        }
        if (cur->rchild==nullptr) cur->rtag=true;
        if (pre->rtag) pre->rchild=cur;
        pre=cur;
        threaded(cur->rchild, pre);//右子树 
    }
}

void TBT::addHeader()
{
    header->ltag=true;
    header->rtag=true;
    header->lchild=header->rchild=header;
    Node *pre=header;
    threaded(root, pre);//线索化
    pre->rtag=true;
    pre->rchild=header;
    header->lchild=pre;
}

Node *TBT::getSuc(Node *node)
{
    if (node->rtag) return node->rchild;
    Node *p=node->rchild;
    while (p->ltag==false) p=p->lchild;
    return p;
}

Node *TBT::getPre(Node *node)
{
    if (node->ltag) return node->lchild;
    Node *p=node->lchild;
    while (p->rtag==false) p=p->rchild;
    return p;
}

void TBT::print()
{
    cout<<"中序遍历为：";
    Node *p=header->rchild;
    while (p!=header)
    {
    	printf("%d ",p->data);
        p=getSuc(p);
    }
}

int main()
{
    TBT Tree;
    Tree.addHeader();
    Tree.print();
}
