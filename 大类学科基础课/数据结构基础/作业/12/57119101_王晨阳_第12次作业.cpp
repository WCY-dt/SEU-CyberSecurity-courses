#include<queue>
#include<algorithm>

typedef int eleType;
struct Node{
	eleType v;
	Node* ptr=NULL;
	Node()
	{
	}
	Node(eleType V)
	{
		v=V;
	}
	Node(Node* n)
	{
		this->v=n->v;
		this->ptr=n->ptr;
	}
};

Node node[100];

void create()
{
	for (int i=0;i<=6;i++)
		node[i].v=i;
	node[0].ptr=new Node(1);
	node[0].ptr->ptr=new Node(2);
	node[0].ptr->ptr->ptr=new Node(3);
	node[1].ptr=new Node(4);
	node[1].ptr->ptr=new Node(5);
	node[3].ptr=new Node(6);
/*example:
        0
      / | \
     1  2  3
    /|     | 
   4 5     6
*/	
}

void destroy()
{
	delete node[3].ptr;
	delete node[1].ptr->ptr;
	delete node[1].ptr;
	delete node[0].ptr->ptr->ptr;
	delete node[0].ptr->ptr;
	delete node[0].ptr;
}

void bfs(Node n)
{
	std::queue<int> q;
	int tmp=0;
	q.push(0);
	while (!q.empty())
	{
		tmp=q.front();
		q.pop();
		printf("%-3d",node[tmp].v);
		Node tmptr(node[tmp]);
		while (tmptr.ptr!=NULL)
		{
			tmptr=tmptr.ptr;
			q.push(tmptr.v);
		}
	}
}

void dfs(int tmp)
{
	printf("%-3d",node[tmp].v);
	Node* tmptr=&node[tmp];
	while (tmptr->ptr!=NULL)
	{
		tmptr=tmptr->ptr;
		dfs(tmptr->v);
	}
}

int main()
{
	create();
	printf("BFS: ");
	bfs(node);
	printf("\nDFS: ");
	dfs(0);
	destroy();
}
