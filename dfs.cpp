#include<iostream>
#include<unordered_map>
#include<vector>
#include<omp.h>
#include<chrono>

using namespace std;

std::unordered_map<int, vector<int>> graph;
std::vector<bool> visited;

void serial_dfs(int node) {
    if(!visited[node]) {
        visited[node] = true;
        cout<<node<<" ";
        
        for (int adjacent : graph[node]) {
            if (!visited[adjacent]) {
                serial_dfs(adjacent);
            }
        }
    }
}

void parallel_dfs(int node) {
    if (!visited[node]) {
        // Mark the node as visited before spawning tasks
        #pragma omp critical
        {
            visited[node] = true;
        }

        cout << node << " ";
        
        #pragma omp parallel for
        for (int adjacent : graph[node]) {
            #pragma omp task
            {
                parallel_dfs(adjacent);
            }
        }
    }
}


int main() {
    int edges, nodes, initial;

    cout<<"Enter No of Nodes: ";
    cin>>nodes;
    visited.assign(nodes, false);

    cout<<"Enter No of Edges: ";
    cin>>edges;

    cout<<"Enter Pair of Edges:"<<endl;
    for(int i=0; i<edges; i++) { 
        int u, v;
        cin>>u>>v;
        graph[u].push_back(v);
    }

    cout<<"Enter Start Node: ";
    cin>>initial;

    auto start_s = chrono::high_resolution_clock::now();
    cout<<"Serial DFS: ";
    serial_dfs(initial);
    auto end_s = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_s = end_s - start_s;
    cout<<"\nTime: "<<time_s.count()<<"ms"<<endl;
    
    visited.assign(nodes, false);

    auto start_p = chrono::high_resolution_clock::now();
    cout<<"Parallel DFS: ";
    parallel_dfs(initial);
    auto end_p = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_p = end_p - start_p;
    cout<<"\nTime: "<<time_p.count()<<"ms"<<endl;

    return 0;
}