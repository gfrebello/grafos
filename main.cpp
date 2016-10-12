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
    vector<int> m_degList;
    vector<AdjList> m_adjArray;
    vector<vector<bool> > m_adjMatrix;
    Graph(int V, bool type);
    AdjListNode* newNode(int dest);
    void addEdge(int src, int dest);
    void print();
    float degMean();
    float relFrequency(int degree);
};
 
Graph::Graph(int V, bool type) {
    m_V = V;
    m_E = 0;
    m_type = type;

    for(int i=0;i<V;i++) m_degList.push_back(0);

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

	//Add a degree unit for each side of edge
	m_degList[src]++;
	m_degList[dest]++;

	if(m_type==0) {
		//Adjency List
		AdjListNode* newNode = this->newNode(dest);
		newNode->next = m_adjArray[src].head;
		m_adjArray[src].head = newNode;

		// Since graph is undirected, add an edge from dest to src also
		newNode = this->newNode(src);
		newNode->next = m_adjArray[dest].head;
		m_adjArray[dest].head = newNode;
	} else {
		//Adjency Matrix
		m_adjMatrix[src][dest]=1;

		// Since graph is undirected, add an edge from dest to src also
		m_adjMatrix[dest][src]=1;	
	}
}

float Graph::degMean() {
     return float(2*m_E/m_V);
}

float Graph::relFrequency (int degree) {
    int n=0;
    for (int i=0;i<m_degList.size();i++)
        if(m_degList[i]==degree) n++;
    return float(n)/m_V;
}


// An utility function to print the adjacenncy list representation of graph
void Graph::print() {
    if(m_type==0) {
	for (int i=0;i<m_V;i++)  {
	    AdjListNode* pCrawl = m_adjArray[i].head;
	    cout << endl << "Adjency list of vertex " << i << endl << "head";
            while (pCrawl!=NULL) {
	        cout << "-> " << pCrawl->dest+1;
	        pCrawl = pCrawl->next;
	    }
	cout << endl;
	}
    } else {
	for(int i=0;i<m_V;i++) {
	    for(int j=0;j<m_V;j++) {
		cout << m_adjMatrix[i][j] << " ";
	    }
	cout << endl;
	}
    }
}

// Driver program to test above functions
int main(int argc, char** argv) {

    ifstream data;
    data.open("data.txt");

    bool type;
    cout << "Define data structure type: 0-Adjency list; 1-Adjency Matrix" << endl;
    cin >> type;

    // Read V from file and create new graph with V vertexes and 0 edges
    int V;
    data >> V;
    Graph graph(V, type);

    //Dynamically create edges
    int v1,v2;
    while (data >> v1 >> v2)
        graph.addEdge(v1-1,v2-1);

    //Print data
    cout << "Number of vertexes: " <<  graph.m_V << endl;
    cout << "Number of edges: " << graph.m_E << endl;
    cout << "Degree mean: " << graph.degMean() << endl;
    for (int i=0;i<graph.m_V;i++) 
	cout <<  "RelFreq(" << i << "): " << graph.relFrequency(i) << endl;
    graph.print();
    return 0;
}
