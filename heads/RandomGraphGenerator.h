#ifndef RANDOM_GRAPH_GENERATOR_H
#define RANDOM_GRAPH_GENERATOR_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;


// Força a conectividade gerando pesos aleatórios (Grafo Normal)
void forceConnectivityNormal(vector<vector<uint8_t>>& graph, int V) {
    vector<int> nodes(V);
    for (int i = 0; i < V; i++) nodes[i] = i;
    
    // Embaralha os nós usando o algoritmo de Fisher-Yates
    for (int i = V - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(nodes[i], nodes[j]);
    }

    // Cria a corrente conexa
    for (int i = 0; i < V - 1; i++) {
        int u = nodes[i];
        int v = nodes[i + 1];
        uint8_t weight = (rand() % 254) + 1;
        graph[u][v] = weight;
        graph[v][u] = weight;
    }
}

// Força a conectividade calculando distâncias euclidianas (Grafo Geométrico)
void forceConnectivityGeometric(vector<vector<uint8_t>>& graph, int V, const vector<pair<double, double>>& points) {
    vector<int> nodes(V);
    for (int i = 0; i < V; i++) nodes[i] = i;
    
    // Embaralha os nós usando o algoritmo de Fisher-Yates
    for (int i = V - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(nodes[i], nodes[j]);
    }

    // Cria a corrente conexa calculando a distância real no plano
    for (int i = 0; i < V - 1; i++) {

        int u = nodes[i];
        int v = nodes[i + 1];
        double dx = points[v].first - points[u].first;
        double dy = points[v].second - points[u].second;
        uint8_t weight = static_cast<uint8_t>(round(sqrt(dx*dx + dy*dy)));
        
        // Garantia de peso minimo
        if (weight == 0) weight = 1;
        
        graph[u][v] = weight;
        graph[v][u] = weight;
    }
}


// GERAÇÃO DE GRAFO NORMAL
vector<vector<uint8_t>> generateGraphMatrix(int V, double density) {
    vector<vector<uint8_t>> graph(V, vector<uint8_t>(V, 0));

    if (density < 0.0) density = 0.0;
    if (density > 1.0) density = 1.0;

    // Chama a função auxiliar do Grafo Normal
    forceConnectivityNormal(graph, V);

    // Adiciona as arestas extras baseadas na densidade restante
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            if (graph[u][v] > 0) continue;
            
            double randomChance = static_cast<double>(rand()) / RAND_MAX;
            
            if (randomChance < density) {
                uint8_t weight = (rand() % 254) + 1;
                graph[u][v] = weight;
                graph[v][u] = weight;
            }
        }
    }
    return graph;
}

// GERAÇÃO DE GRAFO GEOMÉTRICO
vector<vector<uint8_t>> generateGraphMatrixGeometric(int V, double density) {
    vector<pair<double, double>> points(V, {0, 0});

    if (density < 0.0) density = 0.0;
    if (density > 1.0) density = 1.0;

    // Gera os pontos aleatórios no plano espacial
    for (int v = 0; v < V; v++) {
        points[v].first = (rand() % 181);
        points[v].second = (rand() % 181);
    }

    vector<vector<uint8_t>> graph(V, vector<uint8_t>(V, 0));

    // Chama a função auxiliar do Grafo Geométrico passando os pontos gerados
    forceConnectivityGeometric(graph, V, points);

    // Adiciona as arestas extras baseadas na densidade restante
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            if (graph[u][v] > 0) continue;
            
            double randomChance = static_cast<double>(rand()) / RAND_MAX;

            if (randomChance < density) {
                double dx = points[v].first - points[u].first;
                double dy = points[v].second - points[u].second;
                uint8_t weight = static_cast<uint8_t>(round(sqrt(dx*dx + dy*dy)));

                if (weight == 0) weight = 1;

                graph[u][v] = weight;
                graph[v][u] = weight;
            }
        }
    }
    return graph;
}

#endif // RANDOM_GRAPH_GENERATOR_H