# 哈夫曼树

​          假设有$n$个结点，$n$个结点的权值分别为$w_1,w_2,\cdots,w_n$，构成的二叉树的集合为$F=\left\{T_1,T_2,\cdots,T_n\right\}$，则可构造一棵含有$n$个叶子结点的哈夫曼树。步骤如下：

- 从$F$中选取两棵根结点权值最小的树作为左右子树构造一棵新的二叉树，其新的二叉树的权值为其左右子树根结点权值之和

- 从$F$中删除上一步选取的两棵二叉树，将新构造的树放到$F$中

- 重复前两步，直到$F$只含一棵树为止

![img](https://images2017.cnblogs.com/blog/1258764/201711/1258764-20171107153712716-1590327854.jpg)

​           从上面的步骤可以看出，每次选取$n$个结点中权值最小的两个结点，可以使用**小根堆**来实现。为了保持队列中结点一直按从小到大排序，需要自定义比较函数。*由于优先队列中的数据类型为指针类型，所以不能通过重载运算符`<`来实现*，而是要写一个比较类（代码中的类`Compare`）。

# ADT



```cpp
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
```

# 建树



```cpp
class compare
{
public:
	bool operator()(HT *a,HT *b)
	{
 	   return a->value>b->value; //小根堆
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
```

# 哈夫曼编码

​           我们约定左分支表示字符`0`，右分支表示字符`1`，在哈夫曼树中从根结点开始，到叶子结点的路径上分支字符组成的字符串为该叶子结点的哈夫曼编码。上面代码所创建的哈夫曼树如下所示：

![img](https://images2017.cnblogs.com/blog/1258764/201711/1258764-20171107163801419-709292951.png)

​           可以看出`3`被编码为`00`，`1`为`010`，`2`为`011`,`4`为`10`,`5`为`11`。在这些编码中，任何一个字符的编码均不是另一个字符编码的前缀。

```cpp
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
```

# 完整程序

```cpp
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
 	   return a->value>b->value; //小根堆
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
```

**样例**

- 1.out
```
Generating data by random...10694 26311 1673 27809 19765 2581 12817 28408 26529 17849 4271 1986 17304 29206 12552
Huffman Tree sucessfully built!
Codes:
12552 is encoded as 0000
12817 is encoded as 0001
26311 is encoded as 001
26529 is encoded as 010
27809 is encoded as 011
28408 is encoded as 100
29206 is encoded as 101
17304 is encoded as 1100
17849 is encoded as 1101
19765 is encoded as 1110
 4271 is encoded as 111100
 2581 is encoded as 1111010
 1673 is encoded as 11110110
 1986 is encoded as 11110111
10694 is encoded as 11111
```

