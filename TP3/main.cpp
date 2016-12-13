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

struct Tour {
    vector<Vertex> tour;
    double t_cost;
};

double calcDist(Vertex v1, Vertex v2) {
    return sqrt((v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y));
}

double calcTotalCost (Tour t) {
    double r = 0;
    for(int i=0;i<t.tour.size()-1;i++)
        r+=calcDist(t.tour[i],t.tour[i+1]);
    return r;
}

bool operator== (const Vertex &v,const int &a) {
    return v.index == a;
} 


// Nearest-neighbour algorithm for defining initial tour to be optimized
Tour NearestNeighbour(vector<Vertex> v, int root) {

    Tour r;
    vector<Vertex> unvisited = v;

    int v_index = root;
    r.tour.push_back(v[root]);
    r.t_cost = 0;

    while(!unvisited.empty()) {
        double min_dist = 1000000000;
        Vertex cur_vertex = v[v_index];
        Vertex next_vertex;

        unvisited.erase(find(unvisited.begin(),unvisited.end(),v_index));
        if (unvisited.size() == 0) break;

        for(int i=0; i<unvisited.size(); i++) {
            double dist = calcDist(cur_vertex,unvisited[i]);
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

// Auxiliary function for defining best NearestNeighhbour tour
Tour BestNearestNeighbour (vector<Vertex> v) {
    Tour r, new_t;
    double best_cost = 1e10;
    for(int i=0;i<v.size();i++) {
        new_t = NearestNeighbour(v,i);
        if (new_t.t_cost < best_cost) {
            r = new_t;
            best_cost = new_t.t_cost;
        }
    }
    //cout << r.t_cost << endl;
    return r;
}

// Auxiliary function for performing 2-opt swaps
Tour TwoOPTSwap(Tour t, int p1, int p2) {
    Tour r;
    for(int i=0;i<p1;i++)
        r.tour.push_back(t.tour[i]);
    for(int i=p2;i>=p1;i--)
        r.tour.push_back(t.tour[i]);
    for(int i=p2+1;i<t.tour.size();i++)
        r.tour.push_back(t.tour[i]);
    r.t_cost = calcTotalCost(r);
    return r;
}

// Main 2-opt algorithm implementation
Tour TwoOPT (Tour t) {
    Tour r, cur_t;
    cur_t = t;
    
    cout << "Original cost (Best NN): " << t.t_cost << endl;
    
//    for(int i=0;i<t.tour.size()-1;i++) {
//        cout << "Edge[" << i << "]: " << t.tour[i].index << " " << t.tour[i+1].index << endl;
//    }   
    
    double best_cost = calcTotalCost(t);
    double cost_diff = best_cost;
    while(cost_diff > 0) {
        for(int i=1;i<t.tour.size()-2;i++)
            for(int j=i+1;j<t.tour.size()-1;j++) {
                Tour new_t = TwoOPTSwap(cur_t,i,j);
                if (new_t.t_cost < best_cost) {
                    cur_t = new_t;
                    best_cost = new_t.t_cost;
                }
            }
        cost_diff-=best_cost;
        //cout << cost_diff << endl;
    }
    cout << "Cost after 2-opt: " << best_cost << endl;
    cout << "Tour after 2-opt:" << endl;

    for(int i=0;i<cur_t.tour.size();i++)
        cout << cur_t.tour[i].index+1 << " ";
    return r;        
}

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
    
    Tour t = BestNearestNeighbour(V);
    
    t = TwoOPT(t);
   
    return 0;
}

