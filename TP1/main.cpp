#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdio>
#include <algorithm>
#include <ctime>

using namespace std;
 
bool less_vectors(const vector<int>& a, const vector<int>& b) {
	return a.size() > b.size();
}


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
    vector<int> BFS(int root);
    vector<bool> DFS(int root);
    void CC();
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

vector<int> Graph::BFS(int root) {

	clock_t begin = clock();

	int src = root-1;
	// Initialize vectors and queue
	vector<bool> visited(m_V,0);
	vector<int> parent(m_V,-1);
	vector<int> level(m_V,-1);
	vector<int> explored;
	queue<int> queue;

	//Mark src as visited and add it to the queue
	visited[src] = 1;
	level[src] = 0;
	queue.push(src);

	cout << endl<< "BFS(" << root << ") Running..." << endl;

	while(!queue.empty()) {
		//Dequeue a vertex from queue and print it
		int s = queue.front();
		cout << "Vertex " << s+1 << " explored." << endl;
		queue.pop();

		if (m_type == 0) {
			// Adjency list
			AdjListNode* pCrawl = m_adjArray[s].head;
	        while (pCrawl!=NULL) {
	      		if(visited[pCrawl->dest] == 0) {
					cout << "Vertex " << pCrawl->dest+1 << " discovered." << endl;
	      			visited[pCrawl->dest] = 1;
	      			parent[pCrawl->dest] = s;
	      			level[pCrawl->dest]=level[s]+1;
	      			queue.push(pCrawl->dest);
	      		} 
		        pCrawl = pCrawl->next;
		    }	
		} else {
			//Adjency matrix
			for(int i=0;i<m_V;i++) {
				if(m_adjMatrix[s][i] == 1 && visited[i] == 0) {
					cout << "Vertex " << i+1 << " discovered." << endl;
					visited[i] = 1;
					parent[i] = s;
	      			level[i]=level[s]+1;
					queue.push(i);
				}	
			}
		}
	}

	for (int i=0;i<parent.size();i++) cout << "Parent of " << i+1 << " = " << parent[i]+1 << "; " << "Level " << level[i] << endl;

	clock_t	end = clock();
	double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;

	cout << "-------------------" << endl << "Elapsed time: " << elapsed_time << "s" << endl;

	return explored;
}

vector<bool> Graph::DFS(int root) {
	// Prints parent and level of each vertex and returns a vector of all the visited vertexes

	clock_t begin = clock();

	int src = root-1;
	// Initialize vectors and stack
	vector<bool> visited(m_V,0);
	vector<int> parent(m_V,-1);
	vector<int> level(m_V,-1);
	stack<int> stack;

	//Mark src as visited and add it to the queue
	visited[src] = 1;
	level[src] = 0;
	stack.push(src);

	cout << endl<< "DFS(" << root << ") Running..." << endl;
	while(!stack.empty()) {
		//Pop a vertex from stack and print it
		int s = stack.top();
		cout << "Vertex " << s+1 << " explored." << endl;
		stack.pop();

		if (m_type == 0) {
			// Adjency list
			AdjListNode* pCrawl = m_adjArray[s].head;
	        while (pCrawl!=NULL) {
	      		if(visited[pCrawl->dest] == 0) {
					cout << "Vertex " << pCrawl->dest+1 << " discovered." << endl;
	      			visited[pCrawl->dest] = 1;
	      			parent[pCrawl->dest] = s;
	      			level[pCrawl->dest]=level[s]+1;
	      			stack.push(pCrawl->dest);
	      		} 
		        pCrawl = pCrawl->next;
		    }	
		} else {
			//Adjency matrix
			for(int i=0;i<m_V;i++) {
				if(m_adjMatrix[s][i] == 1 && visited[i] == 0) {
					cout << "Vertex " << i+1 << " discovered." << endl;
					visited[i] = 1;
					parent[i] = s;
	      			level[i]=level[s]+1;
					stack.push(i);
				}	
			}
		}
	}

	for (int i=0;i<parent.size();i++) cout << "Parent of " << i+1 << " = " << parent[i]+1 << "; " << "Level " << level[i] << endl;

	clock_t	end = clock();
	double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;

	cout << "-------------------" << endl << "Elapsed time: " << elapsed_time << "s" << endl;

	return visited;

}

void Graph::CC () {
	//Initialize matrix of connected components
	vector<bool> explored(m_V,0);
	vector<vector<int> > CC;
	vector<bool>::iterator it;

	it = find(explored.begin(),explored.end(),0);
	
	while(it!=explored.end()) {
		vector<bool> visited = DFS(it-explored.begin()+1);
		vector<int> buffer;

		for (int j=0;j<visited.size();j++) {
			if(visited[j] == 1) {
				buffer.push_back(j);
				explored[j] = 1;
			}
		}
		CC.push_back(buffer);
		it = find(explored.begin(),explored.end(),0);
	}

	sort(CC.begin(),CC.end(),less_vectors);

	cout << endl << "--------COMPONENTS--------" << endl;
	cout << "TOTAL: " << CC.size() << endl;
	for(int j=0;j<CC.size();j++) {
		cout << j <<": {";
		for(int k=0;k<CC[j].size();k++)
			cout << "," << CC[j][k]+1;
		cout << "} Size = " << CC[j].size() << endl;
	}

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
    graph.BFS(1);
    graph.DFS(1);
    graph.CC();
    return 0;
}
