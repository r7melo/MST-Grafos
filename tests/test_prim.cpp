#include <iostream>
#include <vector>
#include "../heads/Prim.h" // Importa o arquivo onde está a sua função primMST

using namespace std;

// Função auxiliar para calcular o peso total da MST gerada
int calculateWeights(const vector<int> &parent, const vector<vector<uint8_t>> &graph) {
    int totalWeight = 0;
    for (size_t i = 1; i < graph.size(); i++) {
        if (parent[i] != -1) { 
            totalWeight += graph[parent[i]][i];
        }
    }
    return totalWeight;
}

int main() {

    // Matriz de adjacência
    vector<vector<uint8_t>> graph = { 
        { 0, 2, 0, 6, 0 },
        { 2, 0, 3, 8, 5 },
        { 0, 3, 0, 0, 7 },
        { 6, 8, 0, 0, 9 },
        { 0, 5, 7, 9, 0 } 
    };

    // Execução do Prim
    vector<int> parent = primMST(graph);

    cout << "Custo Total: " << calculateWeights(parent, graph);

    return 0;
}