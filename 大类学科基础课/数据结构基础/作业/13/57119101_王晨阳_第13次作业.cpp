#include<algorithm>
#include<cdtdio>
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
    int uAncestor=Infin,vAncestor=Infin;
    
    //where u & v come from
    for (int i=0; i<Tree.size();i++)
    {
        if (find(Tree[i].begin(),Tree[i].end(),u)!=Tree[i].end())
            uAncestor=i;
        if (find(Tree[i].begin(),Tree[i].end(),v)!=Tree[i].end())
            vAncestor=i;
    }
    
    //![the same tree]?(merge)
    if (uAncestor!=vAncestor)
    {
        for (int i=0;i<Tree[vAncestor].size();i++)
            Tree[uAncestor].push_back(Tree[vAncestor][i]);
        Tree[vAncestor].clear();
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
