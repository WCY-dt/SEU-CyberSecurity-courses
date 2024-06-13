#include<algorithm>
#include<cstdio>


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

/*-------------------algorithm implementation-------------------*/

int Prim(int adjMat[][vCount],vData s)
{
	//initialization
	int totVal=0;
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
        totVal+=adjMat[aux[k].d][k];
        aux[k].c=0;
        for (int i=0;i<vCount;i++)//refresh: info: minimum e
            if (adjMat[k][i]<aux[i].c)
            {
                aux[i].d=k;
                aux[i].c=adjMat[k][i];
            }
    }
    return totVal;
}


/*--------------------------main function--------------------------*/

int main()
{
    printf("Edges:\n");
    printf("\nTotal value = %d",Prim(adjMat,0)); //Prim, start from v 0
    
    return 0;
}
