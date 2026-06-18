#include <iostream>
#include <chrono>
#include <numeric>
#include <random>
#include <ctime>
#include "core/estruturas.h"
#include "core/algoritmos.h"
#include "generators/gerador.h"
#include "io/gerenciador_csv.h"

double calcularPesoTotal(const std::vector<Core::Aresta>& mst) {
    double soma = 0.0;
    for (const auto& aresta : mst) {
        soma += aresta.peso;
    }
    return soma;
}

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::string caminho_saida = "resultados/resultados_pipeline.csv";
    IO::GerenciadorCSV::inicializar(caminho_saida);

    std::vector<std::string> tipos_grafos = {"Normal", "Geometrico"};
    std::vector<int> tamanhos_vertices = {100, 500, 1000, 2500, 5000, 7500, 10000};
    std::vector<int> densidades = {0, 20, 40, 60, 80, 100};

    std::cout << "========================================================\n";
    std::cout << "INICIANDO EXECUÇÃO DO PIPELINE DE ALTA PERFORMANCE (RAM)\n";
    std::cout << "========================================================\n";

    for (const auto& tipo : tipos_grafos) {
        for (int V : tamanhos_vertices) {
            for (int D : densidades) {
                
                std::cout << "Executando -> Tipo: " << tipo << " | V: " << V << " | Densidade: " << D << "%\n";

                for (int rep = 1; rep <= 10; ++rep) {
                    std::vector<Core::Aresta> arestas;
                    std::vector<std::vector<std::pair<int, double>>> adj;

                    if (tipo == "Normal") {
                        Generators::Gerador::gerarGrafoNormal(V, D, arestas, adj, rng);
                    } else {
                        Generators::Gerador::gerarGrafoGeometrico(V, D, arestas, adj, rng);
                    }

                    auto t_inicio_k = std::chrono::high_resolution_clock::now();
                    auto mst_kruskal = Core::algoritmoKruskal(V, arestas);
                    auto t_fim_k = std::chrono::high_resolution_clock::now();
                    
                    double tempo_kruskal = std::chrono::duration<double, std::milli>(t_fim_k - t_inicio_k).count();
                    double peso_kruskal = calcularPesoTotal(mst_kruskal);
                    IO::GerenciadorCSV::salvarLinha(tipo, V, D, rep, "Kruskal", tempo_kruskal, peso_kruskal);

                    auto t_inicio_p = std::chrono::high_resolution_clock::now();
                    auto mst_prim = Core::algoritmoPrim(V, adj);
                    auto t_fim_p = std::chrono::high_resolution_clock::now();
                    
                    double tempo_prim = std::chrono::duration<double, std::milli>(t_fim_p - t_inicio_p).count();
                    double peso_prim = calcularPesoTotal(mst_prim);
                    IO::GerenciadorCSV::salvarLinha(tipo, V, D, rep, "Prim", tempo_prim, peso_prim);

                    std::cout << "    [Repeticao " << rep << "/10] Kruskal: " << tempo_kruskal << " ms | Prim: " << tempo_prim << " ms\n";
                }
            }
        }
    }

    std::cout << "========================================================\n";
    std::cout << "PIPELINE FINALIZADO! Arquivo '" << caminho_saida << "' gerado com sucesso.\n";
    std::cout << "========================================================\n";

    return 0;
}