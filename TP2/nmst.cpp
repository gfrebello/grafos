#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;

// Creating shortcut for an integer pair
typedef  pair<int, int> iPair;

// Structure to represent a graph
struct Graph
{
    int V, E;
    vector< pair<int, iPair> > edges;

    // Constructor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }

    // Utility function to add an edge
    void addEdge(int u, int v, int w)
    {
        edges.push_back({w, {u, v}});
    }

    // Function to find MST using Kruskal's
    // MST algorithm
    int kruskalMST();
    int weight(const char* filex);
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            //every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

 /* Functions returns weight of the MST*/

int Graph::kruskalMST()
{
    int mst_wt = 0; // Initialize result

    // Sort edges in increasing order on basis of cost
    sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(V);

    // Iterate through all sorted edges
    vector< pair<int, iPair> >::iterator it;

    // ofstream filex;
    // filex.open("nmst_rede_colaboracao.txt");
    // filex << "Arestas da MST (origem-destino [peso]):\n";

    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST
            // so print it
            //cout << u << " - " << v << endl;
            // filex << u << "-" << v << "[" << it->first << "]\n";


            // Update MST weight
            mst_wt += it->first;

            // Merge two sets
            ds.merge(set_u, set_v);
        }
    }
    // filex.close();
    return mst_wt;
}

int Graph::weight(const char* graphx){
  ifstream filex;
  filex.open(graphx);
  string linex;
  getline(filex,linex);
  int edgex = 0;
  while (getline(filex,linex)){
    int dest_begin = 0;
    int weight_begin = 0;
    for (int charx=0; charx<linex.size(); charx++){
      string space = " ";
      bool comparing = linex[charx] == space[0];
      if (comparing && dest_begin == 0){
        dest_begin = charx + 1;
      } else if (comparing){
        weight_begin = charx + 1;
      }
    }
    int src = stoi (linex.substr(0,dest_begin-1));
    int dest = stoi (linex.substr(dest_begin, weight_begin-1));
    int weight = stoi (linex.substr(weight_begin, linex.size()));
    this->addEdge(src, dest, weight);
    edgex++;
  }
  filex.close();
  int mst_weight = this->kruskalMST();
  cout << mst_weight << endl;
  return mst_weight;
}

int main(int argc, char *argv[]){

    clock_t begin = clock();
    const char* graphx = argv[1];
    ifstream filex;
    filex.open(graphx);
    string line1;
    string linex;

    getline(filex,line1);
    int n_vertex = stoi(line1);
    int n_edges = 0;
    while (getline(filex,linex)) n_edges++;
    Graph g(n_vertex, n_edges);
    filex.close();

    g.weight(graphx);

    clock_t end = clock();
    cout << double(end-begin)/CLOCKS_PER_SEC << endl;

    return 0;
}
