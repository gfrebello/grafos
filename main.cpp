#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;
 
// A structure to represent an adjacency list node
struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int m_V;
    vector<AdjList> m_adjArray;
    vector<vector<bool> > m_adjMatrix;
    Graph(int V, bool type);
    AdjListNode* newNode(int dest);
    void addEdge(int src, int dest);
};
 
Graph::Graph(int V, bool type) {
    m_V = V;
    
    if(type == 0) {
        for(int i=0;i<V;i++) {
            AdjList newList;
            newList.head = NULL;
            m_adjArray.push_back(newList);        
        }        
    }
    else {
        for(int i=0;i<V;i++) 
            m_adjMatrix.resize(V);
    };
}

AdjListNode* Graph::newNode (int dest) {
    AdjListNode* newNode = new AdjListNode;
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

void Graph::addEdge(int src, int dest) {
    AdjListNode* newNode = this->newNode(dest);
    newNode->next = this->m_adjArray[src].head;
    this->m_adjArray[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = this->newNode(src);
    newNode->next = this->m_adjArray[dest].head;
    this->m_adjArray[dest].head = newNode;
}


// A utility function to print the adjacenncy list representation of graph
void printGraph(Graph graph)
{
    int v;
    for (v = 0; v < graph.m_V; ++v)
    {
        AdjListNode* pCrawl = graph.m_adjArray[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl!=NULL)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Driver program to test above functions
int main()
{
    // create the graph given in above fugure
    bool type = 1;
    int V = 5;
    Graph graph(V, type);
    cout << graph.m_V << endl;
    //cout << graph.m_adjArray[2].head << endl;
    //graph.addEdge(0, 1);
    //graph.addEdge(0, 3);
    //graph.addEdge(1, 4);
    //graph.addEdge(2, 0);

    //cout << graph.m_adjArray[4].head << endl;
    //printGraph(graph);
    cout << graph.m_adjMatrix[0][0] << endl;
    return 0;
}