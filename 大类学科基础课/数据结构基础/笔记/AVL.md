# AVL树

 

　　AVL树是一种**高度**平衡的(height balanced)二叉搜索树：对每一个结点$x$，$x$的左子树与右子树的高度差(平衡因子)至多为$1$。有人也许要问：为什么要有AVL树呢？它有什么作用呢？我们先来看看二叉搜索树吧（因为AVL树本质上是一棵二叉搜索树），假设有这么一种极端的情况：二叉搜索树结点的插入顺序为`1,2,3,4,5`，也就是：

![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520171547685-696530838.png)

　　显而易见，这棵二叉搜索树已经其退化成一个链表了，也就是说，它在查找上的优势已经全无了——在这种情况下，查找一个结点的时间复杂度是$O\left(n\right)$！如果这棵二叉搜索树是AVL树，在插入顺序仍为`1,2,3,4,5`的情况下，树的形状如下图：

![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520172116888-215382961.png)

　　可以看出，AVL树基本操作的最坏时间复杂度要比普通的二叉搜索树低—— 除去可能的插入操作外（我们将假设懒惰删除），它是$O\left(\log{n}\right)$。而插入操作隐含着困难的原因在于，插入一个节点可能破坏AVL树的性质（例如，将6插入到上图的AVL树中会破坏根节点2的平衡条件），如果发生这种情况，就要在插入操作结束之前恢复平衡的性质。事实上，这总可以通过对树进行简单的修正来做到，我们称其为**旋转**。

# ADT

```cpp
struct AVLNode 
{
    AVLNode() 
        : val(0), left(nullptr), right(nullptr) 
    {}
    AVLNode(int v)
        : val(v), left(nullptr), right(nullptr)
    {}
    int val;    //data
    // int height;  //当前结点高度
    AVLNode* left;
    AVLNode* right;
};
```



# 旋转

　　在AVL树中，假设有一个结点的平衡因子为$2$（最大就是$2$，因为结点是一个一个地插入到树中的，一旦出现不平衡的状态就会立即进行调整），我们把这个必须重新平衡的结点叫做**被破坏点$\alpha$**。这种不平衡只可能是下面四种情况造成的：

-  对$\alpha$的左儿子的左子树进行了一次插入，即`LL`情况。
- 对$\alpha$的左儿子的右子树进行了一次插入，即`LR`情况。
- 对$\alpha$的右儿子的左子树进行了一次插入，即`RL`情况。
- 对$\alpha$的右儿子的右子树进行了一次插入，即`RR`情况。

　　情形1和4是关于结点$\alpha$的镜像对称，2和3也是关于结点$\alpha$的镜像对称。因此，理论上只有两种情况：第一种情况是插入发生在“外边”的情况（即`LL`情况或`RR`情况），第二种情况是插入发生在“内部”的情况（即`LR`情况或`RL`情况）。

　　在AVL树中插入结点后，用于保持树的平衡的旋转操作步骤如下：

- 沿着插入点到根结点的路径检查结点的平衡因子，找到途中**第一个不满足AVL树性质的结点**，这个结点就是被破坏点$\alpha$。

- 从被破坏点$\alpha$开始沿着该路径向下再标记连续的两个结点$\beta$、$\gamma$，这三个点就是旋转过程将要涉及的三个点（这些点中**不一定包括插入点**，旋转会使$\beta$或$\gamma$成为新的根，另外两个点作为根的左右儿子，其他结点根据AVL树的性质放置即可）。

- 判断插入点与被破坏点$\alpha$之间的关系属于上述四种情况中的哪一种：如果是插入发生在“外边”的情况(即`LL`的情况或`RR`的情况)，只需要**以$\beta$为新的根结点顶替被破坏点$\alpha$的位置**进行进行一次单旋转即可完成调整；如果是插入发生在“内部”的情形(即`LR`的情况或`RL`的情况)，只需要**以$\gamma$为新的根结点顶替被破坏点$\alpha$的位置**进行稍微复杂的双旋转即可完成调整。

- `LL`:![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520163201732-67455821.png)

```cpp
void LL(AVLNode** t) 
{
    if (t != nullptr)
    {
        AVLNode* tmpPtr = (*t)->left;
        (*t)->left = tmpPtr->right;    //t左子树的右子树作为t的左子树
        tmpPtr->right = *t;
        *t = tmpPtr;
    }
}
```

- `RR`:![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520163649873-2061140040.png)

```cpp
void RR(AVLNode** t)
{
    if (t != nullptr) 
    {
        AVLNode* tmpPtr = (*t)->right;
        (*t)->right = tmpPtr->left;
        tmpPtr->left = *t;
        *t = tmpPtr;
    }
}
```

- `LR`:![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520164058763-1888298767.png)

```cpp
void LR(AVLNode** t) 
{
    RR(&(*t)->left);
    LL(t);
}
```

- `RL`:![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520164516044-164596384.png)

```cpp
void RL(AVLNode** t) 
{
    LL(&(*t)->right);
    RR(t);
}
```

## 实例分析

　　下面给出了一个向AVL树中插入关键字的实例，在已给AVL树的基础上插入$9$（图中虚线表示），沿着插入点$9$到根节点的路径发现第一个高度不平衡的结点$6$，即被破坏点；从被破坏点$6$开始沿着该路径向下标记`6,10,7`为$\alpha,\beta,\gamma$；插入点$9$位于被破坏点$6$的右儿子$10$的左子树上，所以属于RL状况；以$\gamma$结点$7$为新的根节点顶替被破坏点$6$的位置，$\alpha$结点$6$和$\beta$结点$10$分别为γ结点7的左右儿子，其他结点根据AVL树的性质放置即可得到右侧的AVL树。

![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520201957763-371379092.png)![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520202330498-715816848.png)

　　在上面AVL树的基础上继续插入$8$（图中虚线表示），沿着插入点$8$到根节点的路径发现第一个高度不平衡的结点为根节点$4$，即被破坏点；从被破坏点$4$开始沿着该路径向下标记`4,7,10`为$\alpha,\beta,\gamma$；插入点$8$位于被破坏点$4$的右儿子$7$的右子树上，所以属于RR状况；以$\beta$结点$7$为新的根节点顶替被破坏点$4$的位置，$\alpha$结点$4$和$\gamma$结点$10$分别为$\beta$结点$7$的左右儿子，其他结点根据AVL树的性质放置即可得到右侧的AVL树。

　　![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520202746029-1834900486.png)![img](https://images2015.cnblogs.com/blog/739525/201605/739525-20160520203952232-2027813352.png)

　　AVL树是最早的平衡二叉树之一，应用相对其他数据结构较少。**Windows对进程地址空间的管理用到了AVL树**。

# 插入

```cpp
 //插入结点
    void insertNode(AVLNode** t, int v) 
    {   
        //插入结点,使用二级指针改变父节点左右子树指针指向
        if (*t == nullptr) 
            *t = new AVLNode(v); 
        else if (v < (*t)->val) 
        {
            insertNode(&((*t)->left), v);
            int leftH = getHeight((*t)->left);
            int rightH = getHeight((*t)->right);
            //插入到左子树,肯定是左子树高度更高,判断这时平衡因子是否大于1
            if ((leftH - rightH) > 1) 
            {
                if (v < (*t)->left->val) 
                    LL(t);
                else
                    LR(t);
            }
        }
        else if (v > (*t)->val) 
        {
            insertNode(&((*t)->right), v);
            int leftH = getHeight((*t)->left);
            int rightH = getHeight((*t)->right);
            if ((rightH - leftH) > 1) 
            {
                if (v > (*t)->right->val)
                    RR(t);
                else 
                    RL(t);
            }
        }
        else 
            return ;
    }
```

# 删除

```cpp
//找到左子树中最大值结点
int findMaxKeyInLef(AVLNode* node) 
    {
        if (node == nullptr)
            return 0;
        else if (node->right == nullptr)
            return node->val;
        return findMaxKeyInLef(node->right);
    }
    AVLNode* delNodeFromTree(AVLNode** node, int val) 
    {
        if (node == nullptr) 
            return nullptr;
        else if (val < (*node)->val)
        {
            (*node)->left = delNodeFromTree(&(*node)->left, val);
            //判断是否失衡,删了左子树一个结点,所以判断右子树高度是否过高
            if ((getHeight((*node)->right) - getHeight((*node)->left)) > 1) 
                //右子树的左子树高度比右子树的右子树更高,相当于给右子树的右子树插入了新节点,相当于"右右"情况
                if (getHeight((*node)->right->left) > getHeight((*node)->right->right))
                    RL(node);
                else 
                    RR(node);
            return (*node);
        }
        else if (val > (*node)->val)
        {
            (*node)->right = delNodeFromTree(&(*node)->right, val);
            //判断是否失衡,删了右子树一个结点,所以判断左子树高度是否过高
            if ((getHeight((*node)->left) - getHeight((*node)->right)) > 1) 
                //左子树的左子树高度比右子树的右子树更高,相当于给左子树的左子树插入了新节点,相当于"左左"情况
                if (getHeight((*node)->left->left) > getHeight((*node)->left->right))
                    LL(node);
                else 
                    LR(node);
            return (*node);
        }
        else if (val == (*node)->val)
        {
            //如果是叶子节点
            if ((*node)->left == nullptr && (*node)->right == nullptr) 
            {
                delete (*node);
                (*node) = nullptr;
                return (*node);;
            }
            //如果左子树非空,将右子树续接到父节点
            else if ((*node)->left != nullptr) 
            {
                AVLNode* tmp = (*node)->left;
                delete (*node);
                return tmp;
            }
            //如果右子树非空,将左子树续接到父节点
            else if ((*node)->right != nullptr) 
            {
                AVLNode* tmp = (*node)->right;
                delete (*node);
                return tmp;
            }
            //左右子树皆非空
            else 
            {
                //寻找左子树中最大节点,即左子树中最右节点
                //(也可以寻找右子树中最小节点,即右子树中最左节点)
                int maxVal = findMaxKeyInLef((*node)->left);
                //交换这两个节点
                (*node)->val = maxVal;
                //删除那个用来交换的节点
                (*node)->left = delNodeFromTree(&(*node)->left, maxVal);
                return *node;
            }
        }
    }
```

# 完整程序

```cpp
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

struct AVLNode 
{
    AVLNode() 
        : val(0), left(nullptr), right(nullptr) 
    {}
    AVLNode(int v)
        : val(v), left(nullptr), right(nullptr)
    {}
    int val;    //data
    // int height;  //当前结点高度
    AVLNode* left;
    AVLNode* right;
};

class AVLTree 
{
public:
    AVLTree() : root(nullptr) 
    {}
    ~AVLTree()
    {}

    //查找某特定值结点
    AVLNode* findNode(int val) 
    {
        return findNodeInTree(root, val);
    }

    //插入结点,t为插入节点
    void insert(int v) 
    {   
        insertNode(&root, v);
    }

    //删除结点,val是待删除结点data
    void delNode(int val) 
    {
        delNodeFromTree(&root, val);
    }

    //层次遍历
    void traverse()
    {
        if (root != nullptr) 
        {
            queue<AVLNode *> q;
            q.push(root);
            AVLNode* tmpPtr;
            while (!q.empty()) 
            {
                tmpPtr = q.front();
                q.pop();
                cout << tmpPtr->val << ' ';

                if (tmpPtr->left != nullptr)
                    q.push(tmpPtr->left);
                if (tmpPtr->right != nullptr) 
                    q.push(tmpPtr->right);
            }
            cout << endl;
        }
    }

    //求结点所在高度(只有一个根结点时高度为1)
    int getHeight(AVLNode* t) 
    {
        int leftHeight, rightHeight;
        if (t != nullptr) 
        {
            leftHeight = getHeight(t->left);
            rightHeight = getHeight(t->right);

            return (leftHeight > rightHeight) ? 
                (leftHeight + 1) : (rightHeight + 1);
        }
        else 
            return 0;
    }
private:
    //左左情况旋转(t是失衡结点)
    void LL(AVLNode** t) 
    {
        if (t != nullptr)
        {
            AVLNode* tmpPtr = (*t)->left;
            (*t)->left = tmpPtr->right;    //t左子树的右子树作为t的左子树
            tmpPtr->right = *t;
            *t = tmpPtr;
        }
    }
    //右右情况旋转
    void RR(AVLNode** t)
    {
        if (t != nullptr) 
        {
            AVLNode* tmpPtr = (*t)->right;
            (*t)->right = tmpPtr->left;
            tmpPtr->left = *t;
            *t = tmpPtr;
        }
    }
    //左右情况旋转 (t为失衡结点,新节点位于t的左子树的右子树)
    void LR(AVLNode** t) 
    {
        RR(&(*t)->left);
        LL(t);
    }
    //右左情况旋转
    void RL(AVLNode** t) 
    {
        LL(&(*t)->right);
        RR(t);
    }

    //插入结点
    void insertNode(AVLNode** t, int v) 
    {   
        //插入结点,使用二级指针改变父节点左右子树指针指向
        if (*t == nullptr) 
            *t = new AVLNode(v); 
        else if (v < (*t)->val) 
        {
            insertNode(&((*t)->left), v);
            int leftH = getHeight((*t)->left);
            int rightH = getHeight((*t)->right);
            //插入到左子树,肯定是左子树高度更高,判断这时平衡因子是否大于1
            if ((leftH - rightH) > 1) 
            {
                if (v < (*t)->left->val) 
                    LL(t);
                else
                    LR(t);
            }
        }
        else if (v > (*t)->val) 
        {
            insertNode(&((*t)->right), v);
            int leftH = getHeight((*t)->left);
            int rightH = getHeight((*t)->right);
            if ((rightH - leftH) > 1) 
            {
                if (v > (*t)->right->val)
                    RR(t);
                else 
                    RL(t);
            }
        }
        else 
            return ;
    }

    AVLNode* findNodeInTree(AVLNode* node, int val) 
    {
        if (node != nullptr) 
        {
            if (val < node->val)
                return findNodeInTree(node->left, val);
            else if (val > node->val) 
                return findNodeInTree(node->right, val);
            else 
                return node;
        }
        else 
            return nullptr;
    }

    int findMaxKeyInLef(AVLNode* node) 
    {
        if (node == nullptr)
            return 0;
        else if (node->right == nullptr)
            return node->val;
        return findMaxKeyInLef(node->right);
    }
    AVLNode* delNodeFromTree(AVLNode** node, int val) 
    {
        if (node == nullptr) 
            return nullptr;
        else if (val < (*node)->val)
        {
            (*node)->left = delNodeFromTree(&(*node)->left, val);
            //判断是否失衡,删了左子树一个结点,所以判断右子树高度是否过高
            if ((getHeight((*node)->right) - getHeight((*node)->left)) > 1) 
                //右子树的左子树高度比右子树的右子树更高,相当于给右子树的右子树插入了新节点,相当于"右右"情况
                if (getHeight((*node)->right->left) > getHeight((*node)->right->right))
                    RL(node);
                else 
                    RR(node);
            return (*node);
        }
        else if (val > (*node)->val)
        {
            (*node)->right = delNodeFromTree(&(*node)->right, val);
            //判断是否失衡,删了右子树一个结点,所以判断左子树高度是否过高
            if ((getHeight((*node)->left) - getHeight((*node)->right)) > 1) 
                //左子树的左子树高度比右子树的右子树更高,相当于给左子树的左子树插入了新节点,相当于"左左"情况
                if (getHeight((*node)->left->left) > getHeight((*node)->left->right))
                    LL(node);
                else 
                    LR(node);
            return (*node);
        }
        else if (val == (*node)->val)
        {
            //如果是叶子节点
            if ((*node)->left == nullptr && (*node)->right == nullptr) 
            {
                delete (*node);
                (*node) = nullptr;
                return (*node);;
            }
            //如果左子树非空,将右子树续接到父节点
            else if ((*node)->left != nullptr) 
            {
                AVLNode* tmp = (*node)->left;
                delete (*node);
                return tmp;
            }
            //如果右子树非空,将左子树续接到父节点
            else if ((*node)->right != nullptr) 
            {
                AVLNode* tmp = (*node)->right;
                delete (*node);
                return tmp;
            }
            //左右子树皆非空
            else 
            {
                //寻找左子树中最大节点,即左子树中最右节点
                //(也可以寻找右子树中最小节点,即右子树中最左节点)
                int maxVal = findMaxKeyInLef((*node)->left);
                //交换这两个节点
                (*node)->val = maxVal;
                //删除那个用来交换的节点
                (*node)->left = delNodeFromTree(&(*node)->left, maxVal);
                return *node;
            }
        }
    }



    AVLNode* root;
};

int main() 
{
    AVLTree tree;
    vector<int> tmp = {99, 1, 34, 56, 23, 67, 78, 9, 45, 684, 35, 678, 234, 89, 90, 24, 672, 1, 1, 4};
    for (auto x : tmp) 
    {
        tree.insert(x);
    }
    tree.traverse();

    AVLNode* p = tree.findNode(672);
    if (p == nullptr)
        cout << "672 is not in the tree" << endl;
    else 
        cout << "succeed in finding " << p->val << endl;

    tree.delNode(672);
    tree.traverse();

    p = tree.findNode(672);
    if (p == nullptr)
        cout << "672 is not in the tree" << endl;
    else 
        cout << "succeed in finding " << p->val << endl;
}
```

