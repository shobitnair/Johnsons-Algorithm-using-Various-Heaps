#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define endl "\n"
const int N = 1e6; // Maximum number of nodes.
const int inf = 999999; // macro for inf
int n , d;
vector< pair<int, int> > adj[N]; // Adjacency list to store edges in the form u--> (v,w)
vector<int> h(N); // Modification array for johnsons algorithm

int flag=0;
int choice = 4; //default choice :- fibonacci

clock_t ttt; // Clock Variable.

/////////////////////////////////prototypes ///////////////////////////////////////////////////////////////
//binary heap structure.
struct binary_heap
{
   vector<pair<int , int>> heap;
   void heapify( int i )
   {
      int size = heap.size();
      int small = i;
      int l = 2*i +1; int r = 2*i + 2;
      if( l < size && heap[l].first < heap[small].first)small = l;
      if( r < size && heap[r].first < heap[small].first)small = r;
      if( small != i)
      {
         swap(heap[i] , heap[small]);
         heapify(small);
      }
   }

   void insert( int val , int id)
   {
      if( heap.size() == 0)heap.pb({val , id});
      else
      {
         heap.pb({val , id});
         for(int i = (heap.size()/2)-1 ; i>=0 ; i--)
            heapify(i);
      }
   }

   void pop()
   {
      int sz = heap.size();
      swap(heap[0] , heap[sz-1]);
      heap.pop_back();
      for(int i= (sz/2)-1 ; i>=0 ; i--)
         heapify(i);
   }

   void decrease_key( int val , int id)
   {
      if( heap.size() == 0)heap.pb({val , id});
      else
      {
         heap.pb({val , id});
         for(int i = (heap.size()/2)-1 ; i>=0 ; i--)
            heapify(i);
      }
   }

};

struct binomial_node
{

    int key; // data
    int vertex; // node_vertex
    int degree; // degree of the node
    binomial_node *ls; //left sibling
    binomial_node *rs; //right sibling
    binomial_node *par; //parent
    binomial_node *fc; //first child
    binomial_node *lc; //lat child

    //struct overload
    binomial_node(int data)
    {
        ls = NULL;
        rs = NULL;
        degree = 0;
        key = data;
        fc = NULL;
        lc = NULL;
        par = NULL;
        
    }

    //struct overload.
    binomial_node(int data, int v)
    {
        par = NULL;
        ls = NULL;
        rs = NULL;
        degree = 0;
        vertex = v;
        key = data;
        fc = NULL;
        lc = NULL;
        
    }
};

struct binomial
{
    //function to merge two binomial trees.
    binomial_node *merge(binomial_node *T1, binomial_node *T2)
    {
        // Compare the key_value and merge appropriately.
        if (T1->key >= T2->key) //T2 is smaller and it will be the paremnt
        {
            //do pointer changes to make t2 parent of t1.
            binomial_node *temp = T2->lc;
            T2->lc = T1;
            T1->par = T2;
            T1->rs = temp;
            if (temp != NULL) temp->ls = T1;

            //when two tree of degree 0 are attached then fc and lc are same.
            if (T1->degree == 0)T2->fc = T1; 
            (T2->degree)++; //update degree of T2.

            return T2;
        }
        else
        {
            //T1 is smaller , then do vica versa.
            binomial_node *temp = T1->lc;
            //pointer changes to make t2 as child of t1.
            T1->lc = T2;
            T2->par = T1;
            T2->rs = temp;

            if (temp != NULL) temp->ls = T2;

            if (T1->degree == 0)
            {
                T1->fc = T2;
            }
            //update degree
            (T1->degree)++;
            return T1;
        }
        //function returns the new tree node from the merges tree.
    }


    list<binomial_node *> union_heap(list<binomial_node *> h1, list<binomial_node *> h2)
    {
        list<binomial_node *> newList;
        list<binomial_node*>::iterator l1 = h1.begin();
        list<binomial_node*>::iterator l2 = h2.begin();

        while (l1 != h1.end() && l2 != h2.end())
        {
            if ((*l1)->degree <= (*l2)->degree)
            {
                newList.push_back(*l1);
                l1++;
            }
            else
            {
                newList.push_back(*l2);
                l2++;
            }
        }
        while (l1 != h1.end())
        {
            //cout<<(*l1)->key<<endl;
            newList.push_back(*l1);
            l1++;
        }
        while (l2 != h2.end())
        {
            newList.push_back(*l2);
            l2++;
        }
        return newList;
    }

/*
After the simple merge, we need to make sure that there is at most one Binomial Tree of any order. 
To do this, we need to combine Binomial Trees of the same order. 
We traverse the list of merged roots, we keep track of three-pointers, prev, x and next. 
There can be following 4 cases when we traverse the list of roots.
*/
    list<binomial_node *> modify_heap(list<binomial_node *> heap)
    {
        if (heap.size() <= 1)
        {
            return heap;
        }

        list<binomial_node*>::iterator prev = heap.begin();
        list<binomial_node*>::iterator x = prev; x++;
        list<binomial_node*>::iterator next = x; next++;
        //There will be 4 cases :- 
        /*
            Case 1: Orders of x and next-x are not same, we simply move ahead.
            In following 3 cases orders of x and next-x are same.
            Case 2: If the order of next-next-x is also same, move ahead.
            Case 3: If the key of x is smaller than or equal to the key of next-x, then make next-x as a child of x by linking it with x.
            Case 4: If the key of x is greater, then make x as the child of next.
            
        We iterate throughout the heap and do according the above 4 cases.   
        */
        while (true)
        {
            
            if (next == heap.end())
            {
                if ((*prev)->degree == (*x)->degree)
                {
                    binomial_node *newTree = merge((*prev), (*x));
                    (*prev) = newTree;
                    heap.erase(x);
                    return heap;
                }
                return heap;
            }

            if ((*prev)->degree == (*x)->degree && (*x)->degree != (*next)->degree)
            {
                binomial_node *newTree = merge((*prev), (*x));
                (*prev) = newTree;
                x = heap.erase(x);
                next++;
            }
            else if ((*prev)->degree == (*x)->degree && (*x)->degree == (*next)->degree)
            {
                prev++;x++;
                next++;
            }
            else
            {
                prev++;
                x++;next++;
            }
        }
    }
    //function to insert a node.
    list<binomial_node *> insert(list<binomial_node *> heap, int key, int vertex, vector<binomial_node *> &ref)
    {
        binomial_node *temp = new binomial_node(key, vertex);
        ref[vertex] = temp;
        list<binomial_node *> heap_2;
        //insert heap and adjust the binommial heap by calling merge and union.
        heap_2.push_back(temp);
        heap_2 = union_heap(heap, heap_2);
        heap_2 = modify_heap(heap_2);
        return heap_2;
    }

    //function to obtain min_element from the binomial heap.
    binomial_node *getMin(list<binomial_node *> heap)
    {
        list<binomial_node *>::iterator it = heap.begin();
        binomial_node *min = *it;
        //simply traverse through the list and and retrieve the min_root.
        while (it != heap.end())
        {
            if ((*it)->key < (min)->key)
            {
                min = *it;
            }
            it++;
        }
        return min;
    }

    // Function to delete the Minimum element of Binomial Heap
    list<binomial_node *> deleteMin(list<binomial_node *> heap)
    {
        binomial_node *min = getMin(heap);
        list<binomial_node *> new_heap;
        list<binomial_node*>::iterator it = heap.begin();

        /*first push all binomial trees into new_heap except the binomial tree
        that contains min_val
        */
        while (it != heap.end())
        {
            if (*it != min)
            {
                new_heap.push_back((*it));
            }
            it++;
        }

        list<binomial_node *> heap_2;
        binomial_node *child = min->fc;
        // min->fc = NULL;

        /*traverse through the min_heap and insert the nodes that are no
        the min_val into Heap_2.*/
        while (child != NULL)
        {

            binomial_node *temp = child->ls;
            //cout << child->key << endl;
            child->par = NULL;

            child->rs = NULL;
            child->ls = NULL;

            heap_2.push_back(child);

            child = temp;
        }

        /* Now merge new_heap and hep2 to obtain the new heap that 
        is formed after removing the min-val node , hence the deletemin operation gets
        completed*/
        if (heap_2.size() != 0)
        {
            new_heap = union_heap(heap_2, new_heap);
            new_heap = modify_heap(new_heap);
        }

        return new_heap;
    }

    //function to decrease key from some old value to new value.
    void decreaseKey(binomial_node *node, int newKey, vector<binomial_node *> &ref)
    {
        node->key = newKey;
        binomial_node *par = node->par;

        // Update the heap according to reduced value
        while (par != NULL && par->key > node->key)
        {
            //swap data and change the ref of the node for the vertices
            int parVertex = par->vertex;
            int parKey = par->key;

            par->vertex = node->vertex;
            par->key = node->key;

            ref[node->vertex] = par;

            node->vertex = parVertex;
            node->key = parKey;
            ref[parVertex] = node;

            //go up the tree, and update such that binomial heap property is maintained
            node = par;
            par = node->par;
        }
    }
};

// Class definiton of fibonacci nodes.
class F_node
{
    public:
    // Function prototypes for class fibonacci Node
	bool addChild(F_node * node);
	bool add_SIB(F_node * node);
	bool cut_remove();
	F_node* LMS();
	F_node* RMS();
    // Pointers to parent , right/left sibling , child and x respectivly.
	F_node * par  , *LS , *RS , *ch , *x;
	F_node(int data, int key);
	F_node();

    //Node attributes
    int id;
	int data;
	int key;
	int rank;

};

// Class definition for collection of nodes forming a fibonacci heap.
class F_heap
{
    public:
    // Function prototypes for Fibonacci heap.
    bool add_node(F_node * node);	
	void decreaseKey(int new_key, F_node* vertex);
    F_node* findMin();
	F_heap(F_node *root);~F_heap();
    bool isEmpty();
    F_node* deleteMin();
	F_node* root_arr[1000];
	bool Consolidate_and_Link(F_node* root);	
	F_node * min_ROOT;
	F_heap();
	
};

F_node::F_node(int data, int key)
{
    //using 'This' Pointer.
	this->key = key;  // This corresponds to the distance from the source node.
	this->data = data; //Pointer to the node. Simplifying, we use an int-index to represent each node.
	// pointers to parent , child , left/right siblings.
    par = NULL;
	ch = NULL;
	LS = NULL;
	RS = NULL;
	x = NULL;
    //Number of children for this node.
	rank = 0;

}

/* An overloaded function for declaring node without data and key*/
F_node::F_node()
{
    RS = NULL; //right sibling
    x = NULL;
    rank = 0;
	par = NULL; //parent
    ch = NULL; // child
    LS = NULL; //left sibling
	
}
/* function to obtain the pointer address of left most sibling */
F_node* F_node::LMS()
{
	if(this == NULL)return NULL;
	F_node* temp = this;

    //keep traversing through the left sibling pointer until it reaches null.
	while(temp->LS)temp = temp->LS;
	return temp;
}

/* function to obtain pointer address of right most sibling */
F_node* F_node::RMS()
{
	if(this == NULL)return NULL;
    F_node* temp = this;

    //keep traversing through right sibling pointer until it reaches null.
	while(temp->RS) temp = temp->RS;
	return temp;
}

//Function to add sibling to a destined node.
bool F_node::add_SIB(F_node *node)
{
	F_node* temp = RMS(); //obtain rightmost sibling pointer.
	if(temp == NULL)return false;

    // Set circular pointers appropriately.
	temp->RS = node;
	node->LS = temp;
	node->par = this->par;
	node->RS = NULL;

    //Update rank
	if(par)par->rank++;

	return true;
}

/** This function adds the new_child_node to the parent_node child list. 
 Degree of the parent in incremented by one as well to reflect to the node*/
bool F_node::addChild(F_node *node)
{
	if(ch != NULL)ch->add_SIB(node);
	else
	{
        // If null , then add a completely new rank_1 Fibonacci head.
		ch = node;
		node->par = this;
		rank = 1; 
	}

	return true;
}

// constructs an empty fibonacci heap.
F_heap::F_heap()
{
	min_ROOT = NULL;
}


/** This is the fibonacci heap data structure which has min pointer
    of type fibonacci node and total nodes in the heap*/
F_heap::F_heap(F_node *root)
{
	this->min_ROOT = root;
	min_ROOT->par = NULL;
	min_ROOT->ch = NULL;
	min_ROOT->LS = NULL;
	min_ROOT->RS = NULL;
	min_ROOT->rank = 0;
}

// utility function to check if heap is empty.
bool F_heap::isEmpty()
{
	return (min_ROOT == NULL);
}

// function to add a node into the heap.
bool F_heap::add_node(F_node * node)
{
	if(node == NULL) return false;

    // update min_root if heap is null.
	if(min_ROOT == NULL)min_ROOT = node;
	else
	{
        //Else add sibling node onto min_root in a lazy manner.
		min_ROOT->add_SIB(node);
        //Update min_root if needed.
		if(min_ROOT->key > node->key) min_ROOT = node;
	}
	return true;
}

//This function is used to truncate a child node from a sibling list. It could be a root list as well.
bool F_node::cut_remove()
{
    //update parent rank and do relink child pointers.
	if(par)
	{
		par->rank--;
		if(LS)par->ch = LS;
		else if(RS)par->ch = RS;
		else par->ch = NULL;
	}	
	
    //update sibling pointers
	if(LS) LS->RS = RS;
	if(RS) RS->LS = LS;
	
	LS = NULL;
	RS = NULL;
	par = NULL;

	return true;
}

F_node* F_heap::findMin()
{
	return min_ROOT;
}

/** This function links two nodes as part of the consolidation operation.
    First the larger node is sliced from its sibling list and is then
    added as a child to the smaller node

*/
bool F_heap::Consolidate_and_Link(F_node* root)
{
    //root_arr is our root-list.
	if(root_arr[root->rank] == NULL)
	{
		root_arr[root->rank] = root;
		return false;
	}
	else
	{
        /*
        we scan the root list with the help of min pointer in the heap and combine trees with
        same degree into a single tree. We do this with the help of a auxillary table to see if
        there is a tree with degree already existing in our heap.
        */
		F_node* Link_node = root_arr[root->rank];
		root_arr[root->rank] = NULL;
		
        //We traverse the table each time until all same degree nodes are merged.
		if(root->key < Link_node->key || root == min_ROOT)
		{
			Link_node->cut_remove();
			root->addChild(Link_node);
			if(root_arr[root->rank] != NULL) Consolidate_and_Link(root);
			else root_arr[root->rank] = root;
		}
		else
		{
			root->cut_remove();
			Link_node->addChild(root);
			if(root_arr[Link_node->rank] != NULL) Consolidate_and_Link(Link_node);
			else root_arr[Link_node->rank] = Link_node;
		}
		return true;
	}
}

//decreases the value of a refferenced node and updates the heap via cut_remove.
void F_heap::decreaseKey(int new_key, F_node* vertex)
{
	vertex->key = new_key;

	if(vertex->par != NULL) 
	{
		vertex->cut_remove();
		min_ROOT->add_SIB(vertex);		
	}
	if(vertex->key < min_ROOT->key) min_ROOT = vertex;
}

//function to delete the root of the heap and update the list with new root recursively.
F_node* F_heap::deleteMin()
{
    //make temp to be the left most sibling of the child of Min_root.
	F_node *temp = min_ROOT->ch->LMS();
	F_node *nextTemp = NULL;

    //iterate to find the next_temp for new min-root update
	while(temp != NULL)
	{
		nextTemp = temp->RS; 
		temp->cut_remove();
		min_ROOT->add_SIB(temp);
		temp = nextTemp;
	}

	temp = min_ROOT->LMS();

	if(temp == min_ROOT)
	{
		if(min_ROOT->RS) temp = min_ROOT->RS;
		else
		{
			F_node* out = min_ROOT;
			min_ROOT->cut_remove();
			min_ROOT = NULL;
			return out;
		}
	}
	F_node* out = min_ROOT;
	min_ROOT->cut_remove();
	min_ROOT = temp;

    //Removing the extracted node from the root list.
	for(int i=0; i<100; i++) root_arr[i] = NULL;
	//Iterate till all the children nodes are added to the root list.
	while(temp)
	{
		if(temp->key < min_ROOT->key)
		{
			min_ROOT = temp;
		}

		nextTemp = temp->RS;		
		Consolidate_and_Link(temp);
		temp = nextTemp;
	}

	return out;	
}


////////////////////////////////////////////////////////////prototypes end here//////////////




void djikstra_array(int s) 
{
    vector<int> dist(n+5 , inf);
    vector<int> vis(n+5 , 0);
    dist[s] = 0;
    while(true)
    {
        int x = inf;
        int u = -1;
        for(int i = 0;i < n;i ++)
			if(!vis[i] and x >= dist[i])
			{
                x = dist[i];
                u = i;
            }
		if(u == -1)	break;
		vis[u] = true;
		for(int i =0 ; i<adj[u].size() ; i++)
        {
            int v = adj[u][i].first;
            int w = adj[u][i].second;
            if( dist[v] > dist[u] + w)
                dist[v] = dist[u] + w;
        }
    }
    for(int i=0 ; i<n ; i++)
    {
        if(dist[i] == inf)cout<<dist[i]<<" ";
        else cout<< dist[i] + (h[i] - h[s])<<" ";
    }
    cout<<endl;
}

void djikstra_binary(int s) 
{
    binary_heap bh;
    vector<int> dist(n+5 , inf);
    vector<int> vis(n+5 , 0);
    dist[s] = 0;
        bh.insert(0 , s);
        while(bh.heap.size() != 0)
        {
            int u = bh.heap[0].second;
            bh.pop();
            if(vis[u])continue;
            vis[u] = true;
            for(int i=0 ; i<adj[u].size() ; i++)
            {
                int v = adj[u][i].first;
                int w = adj[u][i].second;
                if(dist[v]>dist[u]+w)
                {
                    dist[v] = dist[u]+w;
                    bh.decrease_key(dist[v] , v);
                }
            }
        }
    

    for(int i=0 ; i<n ; i++)
    {
        if(dist[i] == inf)cout<<dist[i]<<" ";
        else cout<< dist[i] + (h[i] - h[s])<<" ";
    }
    cout<<endl;
}

void djikstra_binomial(int s)
{
    binomial b;
    binary_heap bh;
    vector<int> dist(n+5 , inf);
    vector<int> vis(n+5 , 0);
    dist[s] = 0;
    
        vector<binomial_node *> vertex_map(n); //stores address
        list<binomial_node *> heap;
        heap = b.insert(heap , 0 , s , vertex_map);
        for(int i=0 ; i<n ; i++)if( i != s)heap = b.insert(heap , inf , i , vertex_map);
        while(heap.size() > 0)
        {
            binomial_node *min_node = b.getMin(heap);
            heap = b.deleteMin(heap);
            int u = min_node->vertex;
            if(vis[u])continue;
            vis[u] = true;
            for(int i=0 ; i<adj[u].size() ; i++)
            {
                int v = adj[u][i].first;
                int w = adj[u][i].second;
                if(dist[v]>dist[u]+w)
                {
                    dist[v] = dist[u]+w;
                    b.decreaseKey(vertex_map[v] , dist[v]  , vertex_map);
                }
            }
        }
    for(int i=0 ; i<n ; i++)
    {
        if(dist[i] == inf)cout<<dist[i]<<" ";
        else cout<< dist[i] + (h[i] - h[s])<<" ";
    }
    cout<<endl;
}

void djikstra_fibonacci(int s)
{
    F_heap *heap = new F_heap();
    vector<F_node*> vertex_map(n+5); //stores node address

    for(int i=0 ; i<n ; i++)
    {
        vertex_map[i] = new F_node;
        heap->add_node(vertex_map[i]);
        if(i==s)vertex_map[i]->key = 0;
        else vertex_map[i]->key = inf;
        vertex_map[i]->id = i;
    }

    vector<int> dist(n+5 , inf);
    dist[s] = 0;
    vector<int> vis(n+5 , 0);
    while( ! heap->isEmpty())
    {
        F_node* min_node = heap->deleteMin();
        int u = min_node->id;
        if(vis[u])continue;
        vis[u] = true;
        for(int i=0 ; i< adj[u].size() ; i++)
        {
            int v = adj[u][i].first;
            int w = adj[u][i].second;
            if( dist[v] > dist[u]+w)
            {
                dist[v] = dist[u]+w;
                heap->decreaseKey(dist[v] , vertex_map[v]);
            }
        }

    }

    for(int i=0 ; i<n ; i++)
    {
        if(dist[i] == inf)cout<<dist[i]<<" ";
        else cout<<dist[i] + h[i] - h[s]<<" ";
    }cout<<endl;

}

void bellmann(int s)
{
    h.assign(n+5 , inf);
    h[s] = 0;

	for (int k = 0; k<=n; ++k)
	{
        int change=0;
        for(int i=0 ; i<=n ; i++)
		for (int j = 0; j < adj[i].size(); ++j)
		{
			int u = i;
			int v = adj[i][j].first;
			int w = adj[i][j].second;
            
			if (h[u] != inf && h[u] + w < h[v])
            {
                h[v] = h[u] + w;
                change++;
            }
		}
        if(change == 0)break;
	}
    for( int i=0 ; i<=n ; i++)
    {
        for(int j=0 ; j<adj[i].size() ; j++)
        {
            int u = i;
            int v = adj[i][j].first;
            int w = adj[i][j].second;
            if(h[u] != inf && h[u] + w < h[v])
            {
                flag = 1;
                break;
            }
        }
    }
}

void johnson()
{
    //Johnsons algorithm

    //first create a new vertex and add zero degree edges to every other node.
    int s = n;
    for(int i=0 ; i<n ; i++)
    {
        adj[s].pb(make_pair(i,0));
    }

    // Call- bellmann ford onto this new source node and obtain a modify-array H.
    bellmann(s);
    if(flag) //If negative cycle present during bellman ford.
    {
        cout<<"-1\n";
        return;
    }

    //update the edge_weights using the modify_array H , new weight = old_weight + H[u] - H[v];
    for(int i=0 ; i<n ; i++)
    {
        for(int j=0 ; j<adj[i].size() ; j++)
        {
            int u , v , w;
            u = i;
            v = adj[i][j].first;
            w = adj[i][j].second;
            if(w == 0 || w == inf)continue;
            adj[i][j].second = w + h[u] - h[v];
        }
    }

    //djikstra based on command-line choice.
    if(choice == 1)for(int i=0 ; i<n ; i++)djikstra_array(i);
    else if(choice ==2 )for(int i=0 ; i<n ; i++)djikstra_binary(i);
    else if(choice == 3)for(int i=0 ; i<n ; i++)djikstra_binomial(i);
    else for(int i=0 ; i <n ; i++)djikstra_fibonacci(i);

}

int main(int argc , char* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    choice = atoi(argv[1]);
    int t;
    cin>>t;
    vector<double> time;
    while(t--)
    {
        cin>>n>>d;
        for(int i=0 ; i<n+5 ; i++)adj[i].clear();
        flag=0;
        int neg = 0;

        for(int i=0 ; i<n ; i++)
        {
            for(int j=0 ; j<n ; j++)
            {
                int val; cin>>val;
                if(val <0 )neg = 1;
                if(i!=j && val!=inf)
                {
                    adj[i].pb(make_pair(j,val));
                }
            }
        }
        
        //if directed then only dijkstra is needed.
        if( d == 0)
        {
            ttt = clock(); 
            if(neg != 0)
            {
                cout<<"-1\n";
                continue;
            }
            h.assign(n+5 , inf);
            ttt = clock();
            if(choice == 1)for(int i=0 ; i<n ; i++)djikstra_array(i);
            else if(choice ==2 )for(int i=0 ; i<n ; i++)djikstra_binary(i);
            else if(choice == 3)for(int i=0 ; i<n ; i++)djikstra_binomial(i);
            else for(int i=0 ; i <n ; i++)djikstra_fibonacci(i);
            ttt = clock() - ttt; 
            double time_taken = ((double)ttt)/CLOCKS_PER_SEC;
            time.pb(time_taken);
        }
        else
        {
            ttt = clock(); 
            johnson();
            ttt = clock() - ttt; 
            double time_taken = ((double)ttt)/CLOCKS_PER_SEC;
            time.pb(time_taken);
        }

    }
    for(int i=0 ; i<time.size() ; i++)cout<<time[i]<<" ";

}

