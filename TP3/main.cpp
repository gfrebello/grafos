// Falta implementar o loop do 3-opt e consertar o swap (está aumentando o custo)

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace std;

struct Vertex {
    double x;
    double y;
    int index;
};

bool operator== (const Vertex &v,const int &a) {
    return v.index == a;
} 

float calcDist(Vertex v1, Vertex v2) {
    return sqrt((v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y));
}

struct Edge {
    Vertex v1;
    Vertex v2;
    float dist;
    Edge() {}
    Edge(Vertex a, Vertex b) {
        v1 = a;
        v2 = b;
        dist = calcDist(a,b);
    }};

float calcDist(Edge e) {
    return sqrt((e.v1.x-e.v2.x)*(e.v1.x-e.v2.x) + (e.v1.y-e.v2.y)*(e.v1.y-e.v2.y));
}

struct Tour {
    vector<Vertex> tour;
    float t_cost;
};

struct SwapContainer {
    vector<Edge> new_edges;
    float cost_diff;
};

// Auxiliary function for calculating swap with minimum cost out of all possible swaps for a given iteration of 3-opt
SwapContainer ThreeOPTSwap(Edge e1, Edge e2, Edge e3){
    SwapContainer r;
    float orig_cost = calcDist(e1) + calcDist(e2) + calcDist(e3);
    vector<float> l_costs;
    
    l_costs.push_back(calcDist(e1.v1,e2.v1) + calcDist(e1.v2,e3.v1) + calcDist(e2.v2,e3.v2));
    l_costs.push_back(calcDist(e1.v1,e2.v1) + calcDist(e1.v2,e3.v2) + calcDist(e2.v2,e3.v1));
    l_costs.push_back(calcDist(e1.v1,e2.v2) + calcDist(e1.v2,e3.v1) + calcDist(e2.v1,e3.v2));
    l_costs.push_back(calcDist(e1.v1,e2.v2) + calcDist(e1.v2,e3.v2) + calcDist(e2.v1,e3.v1));
    l_costs.push_back(calcDist(e1.v1,e3.v1) + calcDist(e1.v2,e2.v1) + calcDist(e2.v2,e3.v2));
    l_costs.push_back(calcDist(e1.v1,e3.v1) + calcDist(e1.v2,e2.v2) + calcDist(e2.v1,e3.v2));
    l_costs.push_back(calcDist(e1.v1,e3.v2) + calcDist(e1.v2,e2.v1) + calcDist(e2.v2,e3.v1));
    l_costs.push_back(calcDist(e1.v1,e3.v2) + calcDist(e1.v2,e2.v2) + calcDist(e2.v1,e3.v1));
    
    vector<float>::iterator it = min_element(l_costs.begin(), l_costs.end());
    r.cost_diff = orig_cost - *it;
    
    if(r.cost_diff <=0) {
        r.new_edges.push_back(e1);
        r.new_edges.push_back(e2);
        r.new_edges.push_back(e3);
        return r;
    }
    int min_cost_index = distance(l_costs.begin(),it);
    cout << "Diff cost: " << min_cost_index << endl;
    
    switch (min_cost_index) {
        case 0:
            r.new_edges.push_back(Edge(e1.v1,e2.v1)); 
            r.new_edges.push_back(Edge(e1.v2,e3.v1)); 
            r.new_edges.push_back(Edge(e2.v2,e3.v2));
            break;
        case 1:
            r.new_edges.push_back(Edge(e1.v1,e2.v1)); 
            r.new_edges.push_back(Edge(e1.v2,e3.v2)); 
            r.new_edges.push_back(Edge(e2.v2,e3.v1)); 
            break;
        case 2:
            r.new_edges.push_back(Edge(e1.v1,e2.v2)); 
            r.new_edges.push_back(Edge(e1.v2,e3.v1)); 
            r.new_edges.push_back(Edge(e2.v1,e3.v2)); 
            break;
        case 3:
            r.new_edges.push_back(Edge(e1.v1,e2.v2)); 
            r.new_edges.push_back(Edge(e1.v2,e3.v2)); 
            r.new_edges.push_back(Edge(e2.v1,e3.v1)); 
            break;
        case 4:
            r.new_edges.push_back(Edge(e1.v1,e3.v1)); 
            r.new_edges.push_back(Edge(e1.v2,e2.v1)); 
            r.new_edges.push_back(Edge(e2.v2,e3.v2)); 
            break;
        case 5:
            r.new_edges.push_back(Edge(e1.v1,e3.v2)); 
            r.new_edges.push_back(Edge(e1.v2,e2.v2)); 
            r.new_edges.push_back(Edge(e2.v1,e3.v2)); 
            break;
        case 6:
            r.new_edges.push_back(Edge(e1.v1,e3.v2)); 
            r.new_edges.push_back(Edge(e1.v2,e2.v1)); 
            r.new_edges.push_back(Edge(e2.v2,e3.v1)); 
            break;
        case 7:
            r.new_edges.push_back(Edge(e1.v1,e3.v2)); 
            r.new_edges.push_back(Edge(e1.v2,e2.v2)); 
            r.new_edges.push_back(Edge(e2.v1,e3.v1)); 
            break;
}
    return r;
}

// Nearest-neighbour algorithm for defining initial tour to be optimized
Tour NearestNeighbour(vector<Vertex> v, int root) {

    Tour r;
    vector<Vertex> unvisited = v;

    int v_index = root;
    r.tour.push_back(v[root]);
    r.t_cost = 0;

    while(!unvisited.empty()) {
        float min_dist = 1000000000;
        Vertex cur_vertex = v[v_index];
        Vertex next_vertex;

        unvisited.erase(find(unvisited.begin(),unvisited.end(),v_index));
        if (unvisited.size() == 0) break;

        for(int i=0; i<unvisited.size(); i++) {
            float dist = calcDist(cur_vertex,unvisited[i]);
            if (dist < min_dist) {
                min_dist = dist;
                next_vertex = unvisited[i];
            }
        }
        r.t_cost += min_dist;
        r.tour.push_back(next_vertex);
        v_index = next_vertex.index;
    }
    r.t_cost+=calcDist(v[r.tour.back().index],v[root]);    
    r.tour.push_back(v[root]);
    return r;    
}

// Main 3-opt algorithm implementation
Tour ThreeOPT (Tour t) {
    Tour r;
    vector<Edge> edges;

    r.t_cost = t.t_cost;
    
    cout << "Original cost (NN): " << t.t_cost << endl;

    for(int i=0;i<t.tour.size()-1;i++) {
        edges.push_back(Edge(t.tour[i],t.tour[i+1]));
        cout << "Edge[" << i << "]: " << edges[i].v1.index << " " << edges[i].v2.index << endl;
    }   
    
    SwapContainer sc = ThreeOPTSwap(edges[0],edges[2],edges[4]);
    edges[0] = sc.new_edges[0];
    edges[2] = sc.new_edges[1];
    edges[4] = sc.new_edges[2];
    r.t_cost-= sc.cost_diff;

    cout << "New cost (3-opt): " << r.t_cost << endl;

    for(int i=0;i<edges.size();i++) {
        cout << "Edge[" << i << "]: " << edges[i].v1.index << " " << edges[i].v2.index << endl;
    }    
    
    return r;
}

//
//
//For i=0,...,m{
//    for j=0,...,m-1 {
//        for k=0,...,m-2 {
//            calc-3opt(i,j,k);            
//        }
//    }
//}


int main(int argc, char** argv) {
    
    int n_V, x, y;
    int i=0;
    vector<Vertex> V;
    ifstream data;
    data.open("points-100.txt");
    
    data >> n_V;
    
    while(data >> x >> y) {
        Vertex v;
        v.x = x;
        v.y = y;
        v.index = i;
        V.push_back(v);
        i++;
    }
    
    data.close();
    
    Tour t = NearestNeighbour(V,0);
    

    ThreeOPT(t);
    
    cout << "Tour: " << endl;
    for(int i=0;i<t.tour.size();i++)
        cout << t.tour[i].index << ",";
    return 0;
}

