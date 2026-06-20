#ifndef RANDOM_GRAPH_GENERATOR_H
#define RANDOM_GRAPH_GENERATOR_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

// Função geradora de matriz de adjacência aleatória
vector<vector<uint8_t>> generateGraphMatrix(int V, double density) {
    // Inicializa a matriz de tamanho V x V
    vector<vector<uint8_t>> graph(V, vector<uint8_t>(V, 0));

    // Reajuste do intervalo de densidade
    if (density < 0.0) density = 0.0;
    if (density > 1.0) density = 1.0;

    // Percorre a matriz pela metade
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            
            // Fator de probabilidade
            double randomChance = static_cast<double>(rand()) / RAND_MAX;
            
            if (randomChance < density) {
                // Sorteia um peso entre 1 e maxWeight (garante que 0 seja apenas ausência de aresta)
                uint8_t weight = (rand() % 254) + 1;
                
                graph[u][v] = weight;
                graph[v][u] = weight;
            }
        }
    }

    return graph;
}


// Função geradora de matriz de adjacência aleatoria
vector<vector<uint8_t>> generateGraphMatrixGeometric(int V, double density) {

    // Inicializa a lista de vertices
    vector<pair<double, double>> points(V, {0, 0});

    // Reajuste do intervalo de densidade
    if (density < 0.0) density = 0.0;
    if (density > 1.0) density = 1.0;

    // Gera os pontos aleatorios
    for (int v = 0; v < V; v++) {
        points[v].first = (rand() % 181);
        points[v].second = (rand() % 181);
    }

    // Inicializa a matriz de tamanho V x V
    vector<vector<uint8_t>> graph(V, vector<uint8_t>(V, 0));

    // Percorre a matriz pela metade
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            
            // Fator de probabilidade
            double randomChance = static_cast<double>(rand()) / RAND_MAX;
            
            // Se for sorteado
            if (randomChance < density) {
                // Gera os deltas dos pontos
                double dx = points[v].first - points[u].first;
                double dy = points[v].second - points[u].second;
                
                // Mede a distancia entre os pontos
                uint8_t weight = static_cast<uint8_t>(round(sqrt(dx*dx + dy*dy)));

                // Garante a distancia minima
                if (weight == 0) weight = 1;

                // Atribui os pesos na matriz
                graph[u][v] = weight;
                graph[v][u] = weight;
            }
        }
    }

    return graph;
}

#endif // RANDOM_GRAPH_GENERATOR_H