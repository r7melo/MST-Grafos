#include <iostream>
#include <vector>
#include "../heads/Prim.h"

using namespace std;

// Função auxiliar para imprimir a MST gerada pelos vetores do Prim
void printResult(const vector<int> &parent, const vector<vector<int>> &graph) {
    cout << "Aresta \tPeso\n";
    int custoTotal = 0;
    for (size_t i = 1; i < graph.size(); i++) {
        cout << parent[i] << " - " << i << " \t" << graph[parent[i]][i] << " \n";
        custoTotal += graph[parent[i]][i];
    }
    cout << "--------------------------------\n";
    cout << "Custo Total da MST (Vertex): " << custoTotal << "\n";
}

int main() {

    // Matriz de adjacência
    vector<vector<int>> graph = { 
        { 0, 2, 0, 6, 0 },
        { 2, 0, 3, 8, 5 },
        { 0, 3, 0, 0, 7 },
        { 6, 8, 0, 0, 9 },
        { 0, 5, 7, 9, 0 } 
    };

    int V = graph.size();

    // Como as variáveis de controle (parent) morrem dentro da função VertexMST,
    // vamos replicar a lógica aqui na main para poder capturar e imVertexir o resultado.
    vector<int> parent(V, -1);
    vector<int> key(V, INT_MAX);
    vector<bool> mstSet(V, false);

    // Instancia o seu Heap
    BinaryHeap<Vertex> bh;

    key[0] = 0;
    bh.insert({0, 0}); 

    while (!bh.isEmpty()) { 
        Vertex atual = bh.extractMin();
        int u = atual.vertexId; // Usando o vertexId que corrigimos

        if (mstSet[u]) continue;

        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
                bh.insert({v, key[v]}); // Usando o keyWeight implicitamente pelo construtor {}
            }
        }
    }

    // ImVertexe a árvore gerada e o custo total
    printResult(parent, graph);

    return 0;
}