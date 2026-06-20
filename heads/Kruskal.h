#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "UnionFind.h"

using namespace std;

// Comparador de arestas
inline bool comparator(vector<int> &a, vector<int> &b) {
   return a[2] < b[2]; 
}


inline int kruskalsMST(int V, vector<vector<int>> &edges) {
    // Ordena as arestas
    sort(edges.begin(), edges.end(), comparator);
    
    // Percorrer as arestas em ordem classificada
    UnionFind uf(V);
    int cost = 0;
    int count = 0;
    
    for (auto &e : edges) {
        int x = e[0];
        int y = e[1];
        int w = e[2];
        
        // Certifique-se de que não haja ciclos.
        if (uf.find(x) != uf.find(y)) {
            uf.unite(x, y);
            cost += w;
            if (++count == V - 1) break;
        }
    }
    return cost;
}

#endif // KRUSKAL_H