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

// Converte a matriz de adjacencia em lista de arestas
vector<Edge> convertMatrixToEdges(const vector<vector<uint8_t>> &graph) {
    
    vector<Edge> edges;
    int V = graph.size();

    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            // Verifica se existe aresta
            if (graph[u][v] > 0) {
                // Adiciona aresta a lista
                edges.push_back({
                    static_cast<uint16_t>(u), 
                    static_cast<uint16_t>(v), 
                    graph[u][v]
                });
            }
        }
    }
    
    return edges;
}

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