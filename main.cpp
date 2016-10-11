#include <iostream>
	#include <fstream>
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
    int m_E;
    bool m_type;
    vector<AdjList> m_adjArray;
    vector<vector<bool> > m_adjMatrix;
    Graph(int V, bool type);
    AdjListNode* newNode(int dest);
    void addEdge(int src, int dest);
    float degreeMean();
};
 
Graph::Graph(int V, bool type) {
    m_V = V;
    m_E = 0;
    m_type = type;
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
	m_E++;
	if(m_type==0) {
		AdjListNode* newNode = this->newNode(dest);
		newNode->next = m_adjArray[src].head;
		m_adjArray[src].head = newNode;

		// Since graph is undirected, add an edge from dest to src also
		newNode = this->newNode(src);
		newNode->next = m_adjArray[dest].head;
		m_adjArray[dest].head = newNode;
	} else {
		m_adjMatrix[src][dest]=1;

		// Since graph is undirected, add an edge from dest to src also
		m_adjMatrix[dest][src]=1;	
	}
}

float Graph::degreeMean() {
     return float(2*m_E/m_V);
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

    ifstream data;
    data.open("data.txt");

    // Define data structure type: 0-Adjency list; 1-Adjency Matrix
    bool type = 1;

    // Read V from file and create new graph with V vertexes and 0 edges
    int V;
    data >> V;
    Graph graph(V, type);

    cout << "Number of vertexes: " <<  graph.m_V << endl;
    //cout << graph.m_adjArray[2].head << endl;

    //Dynamically create edges
    int v1,v2;
    while (data >> v1 >> v2)
        graph.addEdge(v1-1,v2-1);

    cout << "Number of edges: " << graph.m_E << endl;
    cout << "Degree mean: " << graph.degreeMean() << endl;
    //cout << graph.m_adjArray[4].head << endl;
    //printGraph(graph);
    cout << graph.m_adjMatrix[1][0] << endl;
    return 0;
}
