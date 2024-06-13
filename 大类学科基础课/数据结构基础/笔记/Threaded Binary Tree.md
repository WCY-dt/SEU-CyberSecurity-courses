# 线索化二叉树

​        线索二叉树（Threaded Binary Tree）定义为：一个二叉树通过如下的方法“穿起来”：**所有应该为空的右孩子指针指向该结点在中序序列中的后继，所有应该为空的左孩子指针指向该结点的中序序列的前驱**。那么在有 $N$ 个结点的二叉树中共有 $N+1$ 个空指针，这些空指针就叫做“*线索*”。（提示：在一个有 $N $个结点的二叉树中，每个结点有 $2 $个指针，所以一共有 $2N $个指针，将这 $N$ 个结点连接起来需要 $N-1$ 条线，即使用了 $N-1$ 个指针。所以剩下 $2N-(N-1)=N+1$ 个空指针。）那线索二叉树有何用处呢？由于巧妙地利用了空指针，所以它可以快速地查找到二叉树中某结点的前驱和后继。接下来具体介绍这个数据结构。

　　二叉树虽然是非线性结构，但二叉树的遍历却为二又树的结点集导出了一个线性序列。希望很快找到某一结点的前驱或后继，但不希望每次都要对二叉树遍历一遍，这就需要把每个结点的前驱和后继信息记录下来。为了做到这一点，可在原来的二叉链表中增加一个前驱指针域`pre`和一个后继指针域`suc`，分别指向该结点在某种次序下的前驱结点和后继结点。以中序遍历为例：

![img](https://raw.githubusercontent.com/WCY-dt/PictureBed/master/2020/12/d620585f81fadd370e447a89e40dadad-fd5da6.png)

　　有许多指针是空指针又没有利用。为了不浪费存储空间，利用空的`lchild`域存放结点的前驱结点指针，利用空的`rchild`域存放结点的后继结点指针。为了区别线索和子女指针，在每个结点中设置两个标志`ltag`和`rtag`。以中序线索二叉树为例，如果`ltag==0`，标明`lchild`域中存放的是指向左子女结点的指针，否则`lchild`域中是指向该结点中序下的前驱的线索；如果`rtag==0`，标明`rchild`域中存放的是指向右子女结点的指针，否则`rchild`域中是指向该结点中序下的后继的线索。由于它们只需占用一个二进位，每个结点所需存储空间节省得多。

![img](https://raw.githubusercontent.com/WCY-dt/PictureBed/master/2020/12/d620585f81fadd370e447a89e40dadad-fd5da6.png)

# ADT

我们约定线索化二叉树的`Node`结构及包含的函数如下：

```cpp
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
```

# 二叉树构造

```cpp
TBT::TBT()
{
    header=new Node;
    root=create();
}

Node *TBT::create()
{
    Node *p=nullptr;
    int x;
    cin>>x;
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
```

# 线索化二叉树

```cpp
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
```

`header` 结点的作用就是把线索化后的二叉树串起来，形成一个环。`header `的左孩子指向中序遍历序列的最后一个结点，右孩子指向中序遍历序列的第一个结点，如下图：

![img](https://segmentfault.com/img/remote/1460000021492450)

# 寻找前驱后继

![img](https://img2018.cnblogs.com/blog/1475571/201905/1475571-20190515184728709-521041043.png)



![img](https://img2018.cnblogs.com/blog/1475571/201905/1475571-20190515184821004-1053375207.png)

```cpp
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
```

# 中序遍历

```cpp
void TBT::print()
{
    cout<<"中序遍历为：";
    Node *p=header->rchild;
    while (p!=header)
    {
        cout<<p->data<<" ";
        p=getSuc(p);
    }
}
```

# 完整程序

```cpp
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
    cin>>x;
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
        cout<<p->data<<" ";
        p=getSuc(p);
    }
}

int main()
{
    TBT Tree;
    Tree.addHeader();
    Tree.print();
}
```

**样例**

- 1.in
  ```
  1 2 4 -1 6 -1 7 -1 -1 -1 3 5 -1 -1 -1
  ```
  *输入格式为前序遍历，-1代表没有左/右子树*

- 1.out
  ```
  中序遍历为：4 6 7 2 1 5 3
  ```

