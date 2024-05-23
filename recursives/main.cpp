#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

enum Status {
    NotVisited = 0,
    Visited = 1,
    Processed = 2,
};

inline void left_trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return (!isspace(ch) && ch != '[');
    }));
}

size_t getVertex(string& str, size_t& counter, unordered_map<int, string>& its,
               unordered_map<string, int>& sti) {
    if (sti.contains(str)) return sti[str];
    its[counter] = str;
    sti[str] = counter;
    return counter++;
}

void dfs(int cur, unordered_map<int, vector<int>>& graph, vector<int>& timings, vector<bool>& visited, size_t& ind) {
    visited[cur] = true;
    for (auto &v: graph[cur]) {
        if (visited[v]) continue;
        dfs(v, graph, timings, visited, ind);
    }
    timings[ind--] = cur;
}

void dfs(int cur, unordered_map<int, vector<int>>& graph, vector<bool>& visited, vector<vector<int>>& scc) {
    visited[cur] = true;
    for (auto &v: graph[cur]) {
        if (visited[v]) continue;
        dfs(v, graph, visited, scc);
    }
    scc[scc.size() - 1].push_back(cur);
}

vector<vector<int>> kasaraju(unordered_map<int, vector<int>>& graph, unordered_map<int, vector<int>>& rev, size_t size) {
    vector<int> timesRev(size, -1);
    vector<bool> visitedRev(size, false);
    size_t ind = size - 1;
    for (auto &v: rev) {
        if (visitedRev[v.first]) continue;
        dfs(v.first, rev, timesRev, visitedRev, ind);
    }
    vector<bool> visited(size, false);
    vector<vector<int>> scc;
    for (size_t i = 0; i < size; i++) {
        int cur = timesRev[i];
        if (visited[cur]) continue;
        vector<int> new_vec;    
        scc.push_back(new_vec);
        dfs(cur, graph, visited, scc);
    }
    return scc;
}

void input(ifstream& is) {
    unordered_map<int, string> its;
    unordered_map<string, int> sti;
    unordered_map<int, vector<int>> graph;
    unordered_map<int, vector<int>> revGraph;
    size_t counter = 0;
    string holder;
    while(getline(is, holder)) {
        stringstream ss(holder);
        string holder2;
        getline(ss, holder2, ':');
        size_t head = getVertex(holder2, counter, its, sti);
        while (getline(ss, holder2, ',')) {
            left_trim(holder2);
            size_t edge = getVertex(holder2, counter, its, sti);
            graph[head].push_back(edge);
            revGraph[edge].push_back(head);
        }
        // for (size_t i = 0; i < graph.size(); i++) {
        //     for (size_t j = 0; j < graph[i].size(); j++) {
        //         cout << its[graph[i][j]] << " ";
        //     }
        //     cout << endl;
        // }
    }
    vector<vector<int>> sccs = kasaraju(graph, revGraph, sti.size());
    // for (size_t i = 0; i < sccs.size(); i++) {
    //     for (auto &v: sccs[i]) {
    //         cout << its[v] << " ";
    //     }
    //     cout << endl;
    // }
    vector<bool> is_recursive(sti.size(), false);
    size_t max_ind = 0;
    for (size_t i = 0; i < sccs.size(); i++) {
        if (sccs.size() > max_ind) max_ind = i;
        size_t size = sccs[i].size();
        for (size_t j = 0; j < size; j++) {
            if (size > 1) {
                is_recursive[sccs[i][j]] = true;
            } else {
                int cur = sccs[i][j];
                for (auto& s: graph[cur]) {
                    if (s == cur) {
                        is_recursive[cur] = true;
                        break;
                    }
                }  
            }
        }
    }
    for (auto &v: its) {
        cout << v.second << " is ";
        if (is_recursive[v.first]) cout << "recursive";
        else cout << "not recursive";
        cout << endl;
    }
    cout << "biggest recursive component: " << endl;
    for (size_t i = 0; i < sccs[max_ind].size(); i++) {
        cout << its[sccs[max_ind][i]] << " ";
    }
    cout << endl << "biggest recursive component size: " << sccs[max_ind].size() << endl;
}

int main() {
    ifstream is("input");    
    input(is);
}
