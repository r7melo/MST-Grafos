#include <iostream>
#include "../heads/Prim.h"
#include "../heads/Kruskal.h"
#include "../heads/RandomGraphGenerator.h" 
#include "../heads/Timer.h"

using namespace std;

void testGraph(int V, double density) {
    // Geração do grafo aleatorio
    cout << "[Teste do grafo aleatorio]" << endl;
    vector<vector<uint8_t>> graph = generateGraphMatrix(V, density);

    // Execucao do prim
    vector<int> parent;

    // Calcula o tempo de execução do prim
    Timer::calculateTime([&]() {
        parent = primMST(graph);
    });

    cout << "Custo Total (prim): " << calculateWeights(parent, graph) << " Tempo: " << Timer::getTime() << "ms" << endl;

    // Execucao do kruskal
    vector<Edge> edges = convertMatrixToEdges(graph);
    int weight;

    // Calcula o tempo de execução do kruskal
    Timer::calculateTime([&]() {
        weight = kruskalsMST(V, edges);
    });

    cout << "Custo Total (kruskal): " << weight << " Tempo: " << Timer::getTime() << "ms" << endl;
}

void testGraphGeometric(int V, double density) {
    // Geração do grafo aleatorio
    cout << "[Teste do grafo aleatorio geometrico]" << endl;
    vector<vector<uint8_t>> graph = generateGraphMatrixGeometric(V, density);

    // Execucao do prim
    vector<int> parent;

    // Calcula o tempo de execução do prim
    Timer::calculateTime([&]() {
        parent = primMST(graph);
    });

    cout << "Custo Total (prim): " << calculateWeights(parent, graph) << " Tempo: " << Timer::getTime() << "ms" << endl;

    // Execucao do kruskal
    vector<Edge> edges = convertMatrixToEdges(graph);
    int weight;

    // Calcula o tempo de execução do kruskal
    Timer::calculateTime([&]() {
        weight = kruskalsMST(V, edges);
    });

    cout << "Custo Total (kruskal): " << weight << " Tempo: " << Timer::getTime() << "ms" << endl;
}

int main() {
    srand(time(NULL));

    // Valores de teste
    int V = 5000; 
    double density = 0.5;

    testGraph(V, density);
    testGraphGeometric(V, density);


    return 0;
}