#include<algorithm>
#include<time.h>


/*------------------------constant definition------------------------*/

const int m=3;


/*--------------basic structure & function definition--------------*/

typedef int eT;//element type
typedef std::string rec;

typedef struct BTNode{
    int keyNum;//number of key
    eT key[m+1];//key[0] not in use
    BTNode* parent;
    BTNode* ptr[m+1];//children
    rec* recptr[m+1];
    BTNode()
	{
        keyNum=0;
        parent=NULL;
        for(int i=0;i<m+1;i++)
            ptr[i]=NULL;
    }
}*BT;

typedef struct{
    BT pt;//point to node finded
    int i;//1<=i<=m,key place in node
    int tag;//[find successfully]?1:0
}re;//finding re

BT T=NULL ;

int Search(BT p,int k)//find k in p->key[1..p->keyNum]
{
    int i=1;
    while(i<=p->keyNum&&k>p->key[i]) i++;
    return i;
}

void SearchBT(BT t,int k,re &r)
{
/*  find k in t, use r return (pt,i,tag)
    succeed: tag=1,k=ith key of node pt point to
    fail: tag=0, between ith $ i+1th key of node pt point to*/
    int i=0,found=0;
    BT p=t,q=NULL;//p->root, p will ->node to find£¬q->parents
    while(p!=NULL&&0==found)
	{
        i=Search(p,k);
        if(i<=p->keyNum&&p->key[i]==k) found=1 ;
        else{q=p;p=p->ptr[i-1];}
    }
    if(1==found)//succeed
        r={p,i,1};
    else//fail
        r={q,i,0};
}

void split(BT &q,int s,BT &ap)
{
    int i,j,n=q->keyNum;
    ap=(BTNode*)malloc(sizeof(BTNode));
    ap->ptr[0]=q->ptr[s];
    for(i=s+1,j=1;i<=n;i++,j++)
	{
        ap->key[j]=q->key[i];
        ap->ptr[j]=q->ptr[i];
    }
    ap->keyNum=n-s;
    ap->parent=q->parent;
    for(i=0;i<=n-s;i++)
        if(ap->ptr[i]!=NULL) ap->ptr[i]->parent=ap;
    q->keyNum=s-1;
}

void newRoot(BT &t,BT p,int x,BT ap)
{
    t=(BTNode*)malloc(sizeof(BTNode));
    t->keyNum=1;t->ptr[0]=p;t->ptr[1]=ap;t->key[1]=x;
    if(p!=NULL) p->parent=t;
    if(ap!=NULL) ap->parent=t;
        t->parent=NULL;
}

void Insert(BT &q,int i,int x,BT ap)//key x & new pointer ap --- q->key[i] & q->ptr[i]
{
    int j,n=q->keyNum;
    for(j=n;j>=i;j--)
	{
        q->key[j+1]=q->key[j];
        q->ptr[j+1]=q->ptr[j];
    }
    q->key[i]=x;q->ptr[i]=ap;
    if(ap!=NULL) ap->parent=q;
    q->keyNum++;
}

void insBT(BT &t,int k,BT q,int i)
{
    int x,s,finished=0,needNewRoot=0;
    BT ap;
    if(NULL==q) newRoot(t,NULL,k,NULL);
    else
	{
        x=k;ap=NULL;
        while(0==needNewRoot&&0==finished)
		{
            Insert(q,i,x,ap);
            if(q->keyNum<m)
				finished=1;
            else
			{
                s=(m+1)/2;split(q,s,ap);x=q->key[s];
                if(q->parent!=NULL)
				{
                    q=q->parent;
					i=Search(q,x);
				}
                else needNewRoot=1;
            }
        }
        if(1==needNewRoot)//t == null | splitted
            newRoot(t,q,x,ap);
    }
}

void Remove(BT &p,int i)
{
    int j,n=p->keyNum;
    for(j=i;j<n;j++)
	{
        p->key[j]=p->key[j+1];
        p->ptr[j]=p->ptr[j+1];
    }
    p->keyNum--;
}

void Successor(BT &p,int i)
{
    BT child=p->ptr[i];
    while(child->ptr[0]!=NULL) child=child->ptr[0];
    p->key[i]=child->key[1];
    p=child;
}

void Restore(BT &p, int i,BT &T)
{
    int j;
    BT ap=p->parent;
    if(ap==NULL)//root == null
    {
            T=p;
            p=p->parent;
            return ;
    }
    BT lc,rc,pr;
    int finished=0 ,r=0;
    while(!finished)
    {
        r=0;
        while(ap->ptr[r]!=p) r++;//position of p in ap
        if(r==0)
        {
            r++;
            lc=NULL,rc=ap->ptr[r];
        }
        else if(r==ap->keyNum)
		{
            rc=NULL;
			lc=ap->ptr[r-1];
		}
        else
        {
             lc=ap->ptr[r-1];
			 rc=ap->ptr[r+1];
		}
        if(r>0&&lc!=NULL&&(lc->keyNum>(m-1)/2))//borrow from left brother
        {
            p->keyNum++;
            for(j=p->keyNum;j>1;j--)
            {
                 p->key[j]=p->key[j-1];
                 p->ptr[j]=p->ptr[j-1];
            }
            p->key[1]=ap->key[r];
            p->ptr[1]=p->ptr[0];
            p->ptr[0]=lc->ptr[lc->keyNum];
            if(NULL!=p->ptr[0])
                p->ptr[0]->parent=p;
            ap->key[r]=lc->key[lc->keyNum];
            lc->keyNum--;
            finished=1;
            break;
        }
        else if(ap->keyNum>r&&rc!=NULL&&(rc->keyNum>(m-1)/2))//borrow from right brother
        {
            p->keyNum++;
            p->key[p->keyNum]=ap->key[r];
            p->ptr[p->keyNum]=rc->ptr[0];
            if(NULL!=p->ptr[p->keyNum])
                p->ptr[p->keyNum]->parent=p;
            ap->key[r]=rc->key[1];
            rc->ptr[0]=rc->ptr[1];
            for(j=1;j<rc->keyNum;j++)
            {
                rc->key[j]=rc->key[j+1];
                rc->ptr[j]=rc->ptr[j+1];
            }
            rc->keyNum--;
            finished=1;
            break;
        }
        r=0;
        while(ap->ptr[r]!=p)
            r++;
        if(r>0&&(ap->ptr[r-1]->keyNum<=(m-1)/2))//merge with left brother
        {
            lc=ap->ptr[r-1];
            p->keyNum++;
            for(j=p->keyNum;j>1;j--)
            {
                p->key[j]=p->key[j-1];
                p->ptr[j]=p->ptr[j-1];
            }
            p->key[1]=ap->key[r];
            p->ptr[1]=p->ptr[0];
            ap->ptr[r]=lc;
            for(j=1;j<=lc->keyNum+p->keyNum;j++)
            {
                lc->key[lc->keyNum+j]=p->key[j];
                lc->ptr[lc->keyNum+j]=p->ptr[j];
            }
            if(p->ptr[0])
            {
                for(j=1;j<=p->keyNum;j++)
                if(p->ptr[p->keyNum+j])
					p->ptr[p->keyNum+j]->parent=lc;
            }
            lc->keyNum=lc->keyNum+p->keyNum;
            for(j=r;j<ap->keyNum;j++)
            {
                ap->key[j]=ap->key[j+1];
                ap->ptr[j]=ap->ptr[j+1];
            }
            ap->keyNum--;
            pr=p;free(pr);
            pr=NULL;
            p=lc;
        }
        else//merge with right brother
        {
            rc=ap->ptr[r+1];
            if(r==0)
                r++;
            p->keyNum++;
            p->key[p->keyNum]=ap->key[r];
            p->ptr[p->keyNum]=rc->ptr[0];
            rc->keyNum=p->keyNum+rc->keyNum;
            ap->ptr[r-1]=rc;
            for(j=1;j<=(rc->keyNum-p->keyNum);j++)
            {
                rc->key[p->keyNum+j]=rc->key[j];
                rc->ptr[p->keyNum+j]=rc->ptr[j];
            }
            for(j=1;j<=p->keyNum;j++)
            {
                rc->key[j]=p->key[j];
                rc->ptr[j]=p->ptr[j];
            }
            rc->ptr[0]=p->ptr[0];
            if(p->ptr[0])
            {
                for(j=1;j<=p->keyNum;j++)
                if(p->ptr[p->keyNum+j])    p->ptr[p->keyNum+j]->parent=rc;
            }
            for(j=r;j<ap->keyNum;j++)
            {
                ap->key[j]=ap->key[j+1];
                ap->ptr[j]=ap->ptr[j+1];
            }
            ap->keyNum--;
            pr=p;
            free(pr);
            pr=NULL;
            p=rc;
        }
        ap=ap->parent;
        if(p->parent->keyNum>=(m-1)/2||(NULL==ap&&p->parent->keyNum>0))
            finished=1;
        else if(ap==NULL)//root==null, height-1
        {
            pr=T;
            T=p;
            free(pr);
            pr=NULL;
            finished=1;
        }
        p=p->parent;
    }
}
void DelBT(BT &p,int i,BT &T)//delete ith key of node p
{
    if(p->ptr[i]!=NULL)//!(£¡terminal at bottom )
	{
        Successor(p,i);
        DelBT(p,1,T);
    }
	else
	{
        Remove(p,i);
        if(p->keyNum<(m-1)/2)
            Restore(p,i,T);
    }
}


/*-------------------algorithm implementation-------------------*/

void printBT(BT &p,int deep)
{
    if(p==NULL) return;
    int i;
    for(i=0;i<p->keyNum;i++)
    {
        printBT(p->ptr[i],deep+1);
        for(int i=0;i<deep;i++)
            printf("\t");
        printf("%d\n",p->key[i+1]);
    }
    printBT(p->ptr[i],deep+1);
}

void createBtree()
{
    T=new BTNode ;
    T->keyNum=0;
}

void insET(eT temp)
{
    re p;
    SearchBT(T,temp,p);
    if(p.tag==0)
    {
        insBT(T,temp,p.pt,p.i);
//		printf("\nB-tree now: ");
//		printBT(T);
    }
//	else printf("\nFail! element already in B-tree.\n");
}

bool fET(eT temp,bool pInfo)
{
    re p ;
    SearchBT(T,temp,p);
    if(p.tag)
	{
        if (pInfo)
			printf("success");
        return true;
    }
    if (pInfo)
    	printf("fail");
    return false;
}

/*void delET(eT temp)
{
    re p ;
    SearchBT(T,temp,p);
    if(p.tag)
    {
        DelBT(p.pt,p.i,T);
        printf("Delete success");printBT(T);
    }
    else printf("element not exist");
}*/

void Destory(BT &t)
{
    int i=0;
    if(t!=NULL)
        while(i<t->keyNum)
        {
            Destory(t->ptr[i]);
            free(t->ptr[i]);
            i++;
        }
    free(t);
    t=NULL;
}

int main()
{
	//generate B-tree
	printf("generating elements randomly: \n");
	int tmp;
	srand(time(0));
	createBtree();
	for (int i=1;i<=25;i++)
	{
		tmp=rand()%100;
		if (fET(tmp,false))
			tmp=rand()%100;
		printf("%-3d",tmp);
		insET(tmp);
	}
	
//	print status of B-tree
	printf("\n\nB-tree now: \n");
	printBT(T,0);
	
//	delET(tmp);
	Destory(T);
}
