#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include "../heads/Prim.h"
#include "../heads/Kruskal.h"
#include "../heads/RandomGraphGenerator.h" 
#include "../heads/Timer.h"

using namespace std;

struct Record {
    string nameRecord;
    double constructionTime;
    double executionTime;
    int weightMST;

    // Retorna um JSON
    string toJSON() const {
        return "{\"nameRecord\":\"" + nameRecord + 
               "\",\"constructionTime\":\"" + to_string(constructionTime) + 
               "\",\"executionTime\":\"" + to_string(executionTime) + 
               "\",\"weightMST\":\"" + to_string(weightMST) + "\"}";
    }
};

// Modificamos o nome da função real para o macro poder interceptar
pair<Record, Record> testExecutionImpl(int V, double density, string nomeDaFuncao, function<vector<vector<uint8_t>>(int, double)> methodGenerator) {
    Record recordPrim;
    Record recordKruskal;

    // Vincula o nome do token
    recordPrim.nameRecord = "Prim (" + nomeDaFuncao + ")";
    recordKruskal.nameRecord = "Kruskal (" + nomeDaFuncao + ")";

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

// Macro que extrai o nome da funcao executada
#define testExecution(V, density, func) testExecutionImpl(V, density, #func, func)


int main() {
    srand(time(NULL));

    // Valores de teste
    int V = 5000; 
    double density = 0.5;

    cout << "[Grafico Normal]" << endl;
    auto [primNormal, kruskalNormal] = testExecution(V, density, generateGraphMatrix);
    cout << "Prim   : " << primNormal.toJSON() << endl;
    cout << "Kruskal: " << kruskalNormal.toJSON() << endl;

    cout << "[Grafico Geometrico]" << endl;
    auto [primGeometric, kruskalGeometric] = testExecution(V, density, generateGraphMatrixGeometric);
    cout << "Prim   : " << primGeometric.toJSON() << endl;
    cout << "Kruskal: " << kruskalGeometric.toJSON() << endl;


    return 0;
}