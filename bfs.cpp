
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;

std::unordered_map<int, vector<int>> graph;
std::vector<bool> visited;


void serial_bfs(int node) {
    queue<int> temp_queue;
    temp_queue.push(node);
    
    while (!temp_queue.empty()) {
        int current = temp_queue.front();
        temp_queue.pop();

        if (!visited[current]) {
            visited[current] = true;
            cout << current << " ";

            for (int adjacent : graph[current]) {
                if (!visited[adjacent]) {
                    temp_queue.push(adjacent);
                }
            }
        }
    }
}

void parallel_bfs(int node) {
    queue<int> temp_queue;
    temp_queue.push(node);

    while (!temp_queue.empty()) {
        int current;

            current = temp_queue.front();
            temp_queue.pop();
      

        if (!visited[current]) {

           
            
                visited[current] = true;
            
            cout << current << " ";

           
            #pragma omp parallel for
            for (int adjacent : graph[current]) {
                if (!visited[adjacent]) {
                    
                    #pragma omp critical
                    {
                        temp_queue.push(adjacent);
                    }
                }
            }
        }
    }
}


int main() {
    int edges, nodes, initial;

    cout << "Enter No of Nodes: ";
    cin >> nodes;
    visited.assign(nodes, false);

    cout << "Enter No of Edges: ";
    cin >> edges;

    cout << "Enter Pair of Edges:" << endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
    }

    cout << "Enter Start Node: ";
    cin >> initial;

    auto start_s = chrono::high_resolution_clock::now();
    cout << "Serial BFS: ";
    serial_bfs(initial);
    auto end_s = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_s = end_s - start_s;
    cout << "\nTime: " << time_s.count() << "ms" << endl;
    
    visited.assign(nodes, false);


    auto start_p = chrono::high_resolution_clock::now();
    cout << "Parallel BFS: ";
    parallel_bfs(initial);
    auto end_p = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_p = end_p - start_p;
    cout << "\nTime: " << time_p.count() << "ms" << endl;
  

    return 0;
}
