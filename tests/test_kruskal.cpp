#include <iostream>
#include <vector>
#include "../heads/Kruskal.h"

using namespace std;

int main() {

    // Matriz de adjacência
    vector<vector<uint8_t>> graph = { 
        { 0, 2, 0, 6, 0 },
        { 2, 0, 3, 8, 5 },
        { 0, 3, 0, 0, 7 },
        { 6, 8, 0, 0, 9 },
        { 0, 5, 7, 9, 0 } 
    };

    // Lista de arestas
    // Uma aresta contém origem, destino e peso.
    vector<Edge> edges = convertMatrixToEdges(graph);
    
    cout << "Custo Total (kruskal): " << kruskalsMST(5, edges);
    
    return 0;
}