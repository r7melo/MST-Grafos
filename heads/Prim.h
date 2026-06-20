#ifndef PRIM_H
#define PRIM_H

#include <iostream>
#include <vector>
#include <climits>
#include <cstdint>
#include "BinaryHeap.h"

using namespace std;

// Estrutura que representa o vértice e o seu peso atual para o Heap
struct Vertex {
    int vertexId;
    int keyWeight;

    // Sobrecarga do operador '<' exigida pelo teu heapifyUp e heapifyDown
    bool operator<(const Vertex& outro) const {
        return this->keyWeight < outro.keyWeight;
    }
};

// Função auxiliar para calcular o peso total da MST gerada
inline int calculateWeights(const vector<int> &parent, const vector<vector<uint8_t>> &graph) {
    int totalWeight = 0;
    for (size_t i = 1; i < graph.size(); i++) {
        if (parent[i] != -1) { 
            totalWeight += graph[parent[i]][i];
        }
    }
    return totalWeight;
}

inline vector<int> primMST(vector<vector<uint8_t>> &graph) {
    int V = graph.size();
    
    // Vetor para armazenar a Árvore Geradora Mínima (MST) construída
    vector<int> parent(V, -1);

    // Valores de chave usados para escolher a aresta de menor peso no corte
    vector<int> key(V, INT_MAX);

    // Para representar o conjunto de vértices já incluídos na MST
    vector<bool> mstSet(V, false);

    // Instanciação do teu Heap Binário customizado
    BinaryHeap<Vertex> bh;

    // Sempre inclui o primeiro vértice na MST.
    // Define a chave como 0 para que este vértice seja o primeiro a ser escolhido.
    key[0] = 0;
    bh.insert({0, 0}); 

    // Loop principal da busca
    while (!bh.isEmpty()) { 
        Vertex atual = bh.extractMin();
        int u = atual.vertexId;

        // Estratégia de Deleção Lazy: se o vértice já foi incluído na MST, ignora
        if (mstSet[u]) continue;

        // Adiciona o vértice escolhido ao conjunto da MST
        mstSet[u] = true;

        // Atualiza os valores de chave e os índices dos pais dos vértices adjacentes a u
        for (int v = 0; v < V; v++) {
            // box[u][v] existirá, v não estará na MST e o peso será menor que key[v]
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
                bh.insert({v, key[v]}); // Insere o peso atualizado no teu heap
            }
        }
    }

    // Retorna a MST
    return parent;
}

#endif // PRIM_H