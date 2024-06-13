# Kruskal

此算法可以称为**加边法**，初始最小生成树边数为$0$，每迭代一次就选择一条满足条件的最小代价边，加入到最小生成树的边集合里。 
- 把图中的所有边按代价从小到大排序； 
- 把图中的$n$个顶点看成独立的$n$棵树组成的森林； 
- 按权值从小到大选择边，所选的边连接的两个顶点$u_i,v_i$，应属于两颗不同的树，则成为最小生成树的一条边，并将这两颗树合并作为一颗树。 
- 重复第三步,直到所有顶点都在一颗树内或者有$n-1$条边为止。

![这里写图片描述](https://img-blog.csdn.net/20160714144315409)

# Prim

此算法可以称为**加点法**，每次迭代选择代价最小的边对应的点，加入到最小生成树中。算法从某一个顶点$s$开始，逐渐长大覆盖整个连通网的所有顶点。

- 图的所有顶点集合为$V$；初始令集合$u=\left\{s\right\},v=V−u$;
- 在两个集合$u,v$能够组成的边中，选择一条代价最小的边$\left(u_0,v_0\right)$，加入到最小生成树中，并把$v_0$并入到集合$u$中。
- 重复上述步骤，直到最小生成树有$n-1$条边或者$n$个顶点为止。

由于不断向集合$u$中加点，所以最小代价边必须同步更新；需要建立一个辅助数组`closedge`,用来维护集合$v$中每个顶点与集合$u$中最小代价边信息：

![这里写图片描述](https://img-blog.csdn.net/20160714161107576)

# 完整程序

```cpp
#include<algorithm>
using std::vector;


/*------------------------constant definition------------------------*/

typedef int vData;
const vData Infin=0xFFFFFFF;
const int vCount=6;//number of v
char vName[]={'A','B','C','D','E','F'};//name of v
int adjMat[vCount][vCount]={{Infin,6	,1	  ,5	,Infin,Infin},
/*create a graph manually*/	{6	  ,Infin,5	  ,Infin,3	  ,Infin},
/*using adjacency matrix*/  {1	  ,5	,Infin,5	,6	  ,4	},
							{5	  ,Infin,5	  ,Infin,Infin,2	},
							{Infin,3	,6	  ,Infin,Infin,6	},
							{Infin,Infin,4	  ,2	,6    ,Infin}};


/*--------------basic structure & function definition--------------*/

struct node//Prim auxiliary array
{
    vData d;//data
    int c;//lowest cost
}aux[vCount];

struct edge//e
{
    vData u;
    vData v;
    int c;//cost
};

bool compare(edge A,edge B)//for sort
{
    return A.c<B.c;
}

int minE(node* aux)//minimum e
{
    int Min=Infin,re=-1;
    for (int i=0;i<vCount;i++)
        if (aux[i].c<Min && aux[i].c!=0)
        {
            Min=aux[i].c;
            re=i;
        }
    return re;
}

bool fTree(vData u,vData v,vector<vector<vData>> &Tree)//[find tree]u & v in the same tree?
{
    int index_u=Infin,index_v=Infin;
    
    //where u & v come from
    for (int i=0; i<Tree.size();i++)
    {
        if (find(Tree[i].begin(),Tree[i].end(),u)!=Tree[i].end())
            index_u=i;
        if (find(Tree[i].begin(),Tree[i].end(),v)!=Tree[i].end())
            index_v=i;
    }
    
    //![the same tree]?(merge)
    if (index_u!=index_v)
    {
        for (int i=0;i<Tree[index_v].size();i++)
            Tree[index_u].push_back(Tree[index_v][i]);
        Tree[index_v].clear();
        return true;
    }
    return false;
}


/*-------------------algorithm implementation-------------------*/

void Prim(int adjMat[][vCount],vData s)
{
	//initialization
    for (int i=0;i<vCount;i++)
        aux[i].c=Infin;
    aux[s].d=s;//start from v s
    aux[s].c=0;
    for (int i=0;i<vCount;i++)//auxiliary array 
        if (i!=s)
        {
            aux[i].d=s;
            aux[i].c=adjMat[s][i];
        }
        
    //main procedure
    for (int e=1;e<=vCount-1;e++)//exit: n-1 e
    {
        int k=minE(aux);
        printf("%c--%c\n",vName[aux[k].d],vName[k]);
        aux[k].c=0;
        for (int i=0;i<vCount;i++)//refresh: info: minimum e
            if (adjMat[k][i]<aux[i].c)
            {
                aux[i].d=k;
                aux[i].c=adjMat[k][i];
            }
    }
}

void Kruskal(int adjMat[][vCount])
{
    vector<edge> Edge;
    //initializatize Edge
    edge* temp=NULL;
    for (int i=0;i<vCount;i++)
        for (int j=0; j<i; j++)
            if (adjMat[i][j]!=Infin)
            {
                temp=new edge;
                temp->u=i;
                temp->v=j;
                temp->c=adjMat[i][j];
                Edge.push_back(*temp);
            }
            
    sort(Edge.begin(),Edge.end(),compare);//ascending order
    vector<vector<vData>> Tree(vCount);//independent trees
    for (int i=0; i<vCount; i++)
        Tree[i].push_back(i);

	//main procedure
    for (int i=0;i<Edge.size();i++)
    {
        vData u=Edge[i].u;  
        vData v=Edge[i].v;
        if (fTree(u,v,Tree))//![the same tree]?(output)
        	printf("%c--%c\n",vName[u],vName[v]);
    }
}


/*--------------------------main function--------------------------*/

int main()
{
    printf("Prim:\n");
    Prim(adjMat,0); //Prim, start from v 0
    
    printf("\nKruskal:\n");
    Kruskal(adjMat);//Kruskal
    
    return 0;
}
```