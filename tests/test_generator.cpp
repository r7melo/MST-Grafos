#include <iostream>
#include "../heads/Prim.h"
#include "../heads/Kruskal.h"
#include "../heads/RandomGraphGenerator.h" 

using namespace std;

int main() {
    srand(time(NULL));

    // Valores de teste
    int V = 10; 
    double density = 0.5;

    // Geração do grafo aleatorio
    cout << "[Teste do grafo aleatorio]" << endl;
    vector<vector<uint8_t>> graph = generateGraphMatrix(V, density);

    // Execucao do prim
    vector<int> parent = primMST(graph);
    cout << "Custo Total (prim): " << calculateWeights(parent, graph) << endl;

    // Execucao do kruskal
    vector<Edge> edges = convertMatrixToEdges(graph);
    cout << "Custo Total (kruskal): " << kruskalsMST(V, edges) << endl;



    // Geração do grafo aleatorio geometrico
    cout << "[Teste do grafo aleatorio geometrico]" << endl;
    vector<vector<uint8_t>> graphGeometric = generateGraphMatrixGeometric(V, density);

    // Execucao do prim
    vector<int> parentGeometric = primMST(graphGeometric);
    cout << "Custo Total (prim): " << calculateWeights(parentGeometric, graphGeometric) << endl;

    // Execucao do kruskal
    vector<Edge> edgesGeometric = convertMatrixToEdges(graphGeometric);
    cout << "Custo Total (kruskal): " << kruskalsMST(V, edgesGeometric) << endl;



    return 0;
}