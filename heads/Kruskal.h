#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include "UnionFind.h"

using namespace std;

struct Edge {
    uint16_t u;
    uint16_t v;
    uint8_t weight;

    // Sobrecarga do operador '<'
    bool operator<(const Edge& other) const {
        return this->weight < other.weight;
    }
};



inline int kruskalsMST(int V, vector<Edge> &edges) {
    // Ordena as arestas
    sort(edges.begin(), edges.end());
    
    // Percorrer as arestas em ordem classificada
    UnionFind uf(V);
    int cost = 0;
    int count = 0;
    
    for (auto &e : edges) {        
        // Certifique-se de que não haja ciclos.
        if (uf.find(e.u) != uf.find(e.v)) {
            uf.unite(e.u, e.v);
            cost += e.weight;
            if (++count == V - 1) break;
        }
    }
    return cost;
}

#endif // KRUSKAL_H