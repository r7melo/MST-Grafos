#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include "../heads/Prim.h"
#include "../heads/Kruskal.h"
#include "../heads/RandomGraphGenerator.h" 
#include "../heads/Timer.h"
#include "../heads/Record.h"
#include "../heads/DataExporter.h"

using namespace std;

pair<Record, Record> testExecution(int V, double density, function<vector<vector<uint8_t>>(int, double)> methodGenerator) {
    Record recordPrim;
    Record recordKruskal;

    vector<vector<uint8_t>> graph;
    vector<int> parent;
    vector<Edge> edges;

    // GERAÇÃO DO GRAFO ALEATORIO EM MATRIZ DE ADJACENCIA
    Timer::calculateTime(recordPrim.constructionTime, [&]{
        graph = methodGenerator(V, density);
    });
    
    // CONVERSAO DA MATRIZ DE ADJACENCIA PARA LISTA DE ARESTAS
    Timer::calculateTime(recordKruskal.constructionTime, [&]{
        edges = convertMatrixToEdges(graph);
    });
    recordKruskal.constructionTime += recordPrim.constructionTime;


    // EXECUÇÃO DO TESTE DE [PRIM]
    Timer::calculateTime(recordPrim.executionTime, [&]() {
        parent = primMST(graph);
    });
    recordPrim.weightMST = calculateWeights(parent, graph);
    
    
    // EXECUÇÃO DO TESTE DE [KRUSKAL]
    Timer::calculateTime(recordKruskal.executionTime, [&]() {
        recordKruskal.weightMST = kruskalsMST(V, edges);
    });

    return {recordPrim, recordKruskal};
}


int main() {
    srand(time(NULL));

    // Arquivo de registros
    auto epoch = chrono::system_clock::now().time_since_epoch().count();
    string fullPathResult = "resultados/records_"+to_string(epoch)+".csv"; 

    // Valores de teste
    vector<int> vertices = {100, 500, 1000, 2500, 5000, 7500, 10000};
    vector<double> densities = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0};

    // Faz os testes para todas as combinacoes
    for (int V : vertices) {
        for (double density : densities) {
            for(size_t i=0; i<10; i++) {

                cout << "====== TESTE (V:"<<V<<", density:"<<density<<", r: "<<i<<") ======" << endl;

                cout << "[Grafico Normal]" << endl;
                auto [primNormal, kruskalNormal] = testExecution(V, density, generateGraphMatrix);
                primNormal.nameRecord = "Prim Nomal";
                kruskalNormal.nameRecord = "Kruskal Normal";                
                
                cout << "[Grafico Geometrico]" << endl;
                auto [primGeometric, kruskalGeometric] = testExecution(V, density, generateGraphMatrixGeometric);
                primGeometric.nameRecord = "Prim Geometric";
                kruskalGeometric.nameRecord = "Kruskal Geometric";
                
                DataExporter::appendToCSV(fullPathResult, {primNormal, kruskalNormal, primGeometric, kruskalGeometric});
            }
        }
    }


    return 0;
}