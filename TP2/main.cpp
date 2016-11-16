#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <map>
#include <unistd.h>
#include <typeinfo>
#include <bits/stdc++.h>

#include "binaryheap.hpp"

using namespace std;

bool less_vectors(const vector<int>& a, const vector<int>& b) {
	return a.size() > b.size();
}

// A structure to represent an adjacency list node
struct AdjListNode {
    int dest;
    float weight;
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
    AdjListNode* newNode(int dest, float weight);
    void addEdge(int src, int dest);
    void addEdge(int src, int dest, float weight);
    float BFS(int root);
    int Kruskal();
    vector<float> dijkstraDists;
    vector<bool> DFS(int root);
    vector<bool> Dijkstra(int root);
    vector<int> MinPath(int root, int v, vector<int> parents);
    void CC();
    void print();
    float degMean();
    float distMean();
    float relFrequency(int degree);
};

// Utility function that, given parents vector, returns minimal path between root and v
vector<int> Graph::MinPath(int root, int v, vector<int> parent) {
	int i = v;
	vector<int> path;

	path.push_back(v);

	while( i!=-1 && parent[i] != root) {
		path.push_back(parent[i]);
		i = parent[i];
	}
	path.push_back(root);
	return path;
}

//Implements Dijkstra's algorithm with a heap
vector<bool> Graph::Dijkstra(int root) {
	int i;
	vector<float> dist;
	vector<float> backup;
	vector<int> parent(m_V,-1);
	vector<int> minpath;

	MinHeap heap(m_V);

	for(int i=0;i<m_V;i++) {
		heap.insertKey(1e4);
		dist.push_back(1e4);
		backup.push_back(1e4);
	}

	dist[root] = 0;
	backup[root] = 0;
	heap.decreaseKey(root,0);

	while(!heap.isEmpty()) {

		float d = heap.getMin();
		vector<float>::iterator it = find(backup.begin(),backup.end(),d);
		int u = distance(backup.begin(),it);

		//cout << "d " << d << " " << "u " << u << endl;

		if(distance(dist.end(),it) == 0) cout << "deu ruim";

		AdjListNode* pCrawl;
		pCrawl = m_adjArray[u].head;

        while (pCrawl != NULL) {
      		if(dist[pCrawl->dest] > dist[u] + pCrawl->weight) {
      			heap.decreaseKey(heap.findIndex(dist[pCrawl->dest]),dist[u] + pCrawl->weight);
      			dist[pCrawl->dest] = dist[u] + pCrawl->weight;
      			backup[pCrawl->dest] = dist[pCrawl->dest];
      			parent[pCrawl->dest] = u;
      		}
	        pCrawl = pCrawl->next;
	    }

//	    heap.dump();
//		cout << heap.getMin() << ' ';
		backup[u] = -1;
		heap.extractMin();
		i++;
	}


	//for(int i=0;i<dist.size();i++)
	// cout << dist[0] << ' ';
	// cout << endl;

	minpath = MinPath(root,0,parent);

	// cout << "{";
	// for(int i=0;i<minpath.size();i++)
	// 	cout << minpath[i] << ',';
	// cout << "}" << endl;
  //
	// cout << endl;
  this->dijkstraDists = dist;

	std::vector<bool> v;
	return v;
}


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


AdjListNode* Graph::newNode (int dest, float weight) {
    AdjListNode* newNode = new AdjListNode;
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void Graph::addEdge(int src, int dest) {
	// Unweighed edges

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

void Graph::addEdge(int src, int dest, float weight) {
	// Weighed edges assuming weight > 0

	m_E++;

	//Add a degree unit for each side of edge
	m_degList[src]++;
	m_degList[dest]++;

	if(m_type==0) {
		//Adjency List
		AdjListNode* newNode = this->newNode(dest,weight);
		newNode->next = m_adjArray[src].head;
		m_adjArray[src].head = newNode;

		// Since graph is undirected, add an edge from dest to src also
		newNode = this->newNode(src,weight);
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

float Graph::BFS(int root) {

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

	// cout << endl<< "BFS(" << root << ") Running..." << endl;

	while(!queue.empty()) {
		//Dequeue a vertex from queue and print it
		int s = queue.front();
	//	cout << "Vertex " << s+1 << " explored." << endl;
		queue.pop();

		if (m_type == 0) {
			// Adjency list
			AdjListNode* pCrawl = m_adjArray[s].head;
	        while (pCrawl!=NULL) {
	      		if(visited[pCrawl->dest] == 0) {
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
					visited[i] = 1;
					parent[i] = s;
	      			level[i]=level[s]+1;
					queue.push(i);
				}
			}
		}
	}

  //Distances mean calc
  int dist[m_V];
  float dist_sum = 0;
  for (int i=0; i < m_V; i++){
    int parentx = parent[i];
    dist[i] = level[i];
    while (level[parentx] != 0){
      dist[i] += level[parentx];
      parentx = parent[parentx];
    }
    dist_sum += dist[i];
  }

	//for (int i=9;i<50;i=i+10) cout << "Parent of " << i+1 << " = " << parent[i]+1 << "; " << "Level " << level[i] << endl;


  clock_t	end = clock();
  double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;

  //cout << "-------------------" << endl << "Elapsed time: " << elapsed_time << "s" << endl;
  return dist_sum/m_V;
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
		//cout << "Vertex " << s+1 << " explored." << endl;
		stack.pop();

		if (m_type == 0) {
			// Adjency list
			AdjListNode* pCrawl = m_adjArray[s].head;
	        while (pCrawl!=NULL) {
	      		if(visited[pCrawl->dest] == 0) {
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
					visited[i] = 1;
					parent[i] = s;
	      			level[i]=level[s]+1;
					stack.push(i);
				}
			}
		}
	}

	//for (int i=9;i<50;i=i+10) cout << "Parent of " << i+1 << " = " << parent[i]+1 << "; " << "Level " << level[i] << endl;

	clock_t	end = clock();
	//double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;

	//cout << "-------------------" << endl << "Elapsed time: " << elapsed_time << "s" << endl;

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
		//for(int k=0;k<CC[j].size();k++)
		//	cout << "," << CC[j][k]+1;
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
		        cout << "-> " << pCrawl->dest << "(" << pCrawl->weight << ") ";
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

float Graph::distMean() {
  for (int i=0; i < this->m_V; i++){
    this->Dijkstra(i);
  }
}
//
// struct DisjointSets{
//     int *parent, *rnk;
//     int n;
//
//     DisjointSets(int n){
//         this->n = n;
//         parent = new int[n+1];
//         rnk = new int[n+1];
//
//         for (int i = 0; i <= n; i++){
//             rnk[i] = 0;
//             parent[i] = i;
//         }
//     }
//
//     int find(int u){
//         if (u != parent[u])
//             parent[u] = find(parent[u]);
//         return parent[u];
//     }
//
//     void merge(int x, int y){
//         x = find(x), y = find(y);
//
//         if (rnk[x] > rnk[y])
//             parent[y] = x;
//         else
//             parent[x] = y;
//
//         if (rnk[x] == rnk[y])
//             rnk[y]++;
//     }
// };
//
// int Graph::Kruskal(){
//     int k_weight = 0;
//
//     for (int i=0; i<m_V; i++)
//       sort(m_adjArray.begin(), m_adjArray.end());
//
//     DisjointSets ds(m_V);
//
//     iterator it;
//
//     // ofstream filex;
//     // filex.open("nmst_rede_colaboracao.txt");
//     // filex << "Arestas da MST (origem-destino [peso]):\n";
//
//     for (it=m_adjArray.begin(); it!=m_adjArray.end(); it++){
//         int u = it->second.first;
//         int v = it->second.second;
//
//         int set_u = ds.find(u);
//         int set_v = ds.find(v);
//
//         if (set_u != set_v){
//             k_weight += it->first;
//             ds.merge(set_u, set_v);
//         }
//     }
//     return k_weight;
// }

// Driver program to test above functions
int main(int argc, char** argv) {

    ifstream data;
    data.open("grafo_2.txt");

    ofstream emp_dist;
    emp_dist.open("teste.txt");

    bool type = 0;
    //cout << "Define data structure type: 0-Adjency list; 1-Adjency Matrix" << endl;
    //cin >> type;
    clock_t begin = clock();

    // Read V from file and create new graph with V vertexes and 0 edges
    int V;
    float mean = 0;
    float m;
    data >> V;
    for (int i=0; i < V; i++){
      Graph graph(V, type);

      //Dynamically create edges
      int v1,v2;
      float w;
      while (data >> v1 >> v2 >> w) {
      	if(w<0) {
  			     cout << "Negative weight detected!" << endl;
  			return 0;
      } else if (v1-1 >= i && v2-1 >= i){
          graph.addEdge(v1-1,v2-1, w);
        }
      }
      graph.Dijkstra(i);
      m = 0;
      for (int j=0; j < V-i; j++){
        m += graph.dijkstraDists[j];
      }
      mean += m/V;
      data.close();
      data.open("grafo_2.txt");
      data >> V;

    }
    mean = m/V;
    cout << mean << endl;

    data.close();
    //cout << "edges added" << endl;
    //pause();

    //Print data
    // cout << "Number of vertexes: " <<  graph.m_V << endl;
    // cout << "Number of edges: " << graph.m_E << endl;
    // cout << "Degree mean: " << graph.degMean() << endl;

    // double max = 0;
    // double min = 999999999999;
    //
    // for (int i=0;i<graph.m_V/1000;i++) {
		// emp_dist << i << " " << graph.relFrequency(i) << endl;
		// if (max < graph.relFrequency(i)) max = graph.relFrequency(i);
		// if (min > graph.relFrequency(i)) min = graph.relFrequency(i);
    // }
    //
    // emp_dist.close();

    // cout << "max: " << max << endl;
    // cout << "min: " << min << endl;
    //  graph.print();


    // First Study Case
  	// graph.Dijkstra(9);
  	// graph.Dijkstra(19);
  	// graph.Dijkstra(29);
  	// graph.Dijkstra(39);
  	// graph.Dijkstra(49);


  	clock_t	end = clock();
  	double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;
  	cout << "-------------------" << endl << "Elapsed time: " << elapsed_time << "s" << endl;


    //graph.DFS(1);
    //graph.CC();
    return 0;
}
