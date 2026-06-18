#include "gerador.h"
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <random>

namespace Generators {

    double calcularDistancia(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
        double dx = static_cast<double>(x1) - static_cast<double>(x2);
        double dy = static_cast<double>(y1) - static_cast<double>(y2);
        return std::sqrt(dx * dx + dy * dy);
    }

    void Gerador::gerarGrafoGeometrico(
        int num_vertices, int densidade_percentual, 
        std::vector<Core::Aresta>& arestas, 
        std::vector<std::vector<std::pair<int, double>>>& adj,
        std::mt19937& rng
        ) {
        
        arestas.clear();
        adj.assign(num_vertices, std::vector<std::pair<int, double>>());

        long long max_arestas_extras = ((long long)num_vertices * (num_vertices - 1) / 2) - (num_vertices - 1);
        long long arestas_para_adicionar = (max_arestas_extras * densidade_percentual) / 100;
        long long total_arestas = (num_vertices - 1) + arestas_para_adicionar;

        arestas.reserve(total_arestas);
        int grau_estimado = static_cast<int>((2 * total_arestas) / num_vertices) + 10;
        for (int i = 0; i < num_vertices; ++i) {
            adj[i].reserve(grau_estimado);
        }

        struct Ponto8Bit { uint8_t x, y; };
        std::vector<Ponto8Bit> pontos(num_vertices);
        std::uniform_int_distribution<int> dist_coord(0, 255);
        for (int i = 0; i < num_vertices; ++i) {
            pontos[i].x = static_cast<uint8_t>(dist_coord(rng));
            pontos[i].y = static_cast<uint8_t>(dist_coord(rng));
        }

        for (int i = 0; i < num_vertices - 1; ++i) {
            double peso = calcularDistancia(pontos[i].x, pontos[i].y, pontos[i + 1].x, pontos[i + 1].y);
            
            arestas.push_back({i, i + 1, peso});
            adj[i].push_back({i + 1, peso});
            adj[i + 1].push_back({i, peso});
        }

        std::uniform_int_distribution<int> dist_vertice(0, num_vertices - 1);
        long long adicionadas = 0;
        long long tentativas_max = arestas_para_adicionar * 3; 
        
        while (adicionadas < arestas_para_adicionar && tentativas_max-- > 0) {
            int u = dist_vertice(rng);
            int v = dist_vertice(rng);

            if (u != v) {
                if (u > v) std::swap(u, v);
                
                double peso = calcularDistancia(pontos[u].x, pontos[u].y, pontos[v].x, pontos[v].y);
                
                arestas.push_back({u, v, peso});
                adj[u].push_back({v, peso});
                adj[v].push_back({u, peso});
                adicionadas++;
            }
        }
    }

    void Gerador::gerarGrafoNormal(
        int num_vertices, int densidade_percentual, 
        std::vector<Core::Aresta>& arestas, 
        std::vector<std::vector<std::pair<int, double>>>& adj,
        std::mt19937& rng
        ) {
        
        arestas.clear();
        adj.assign(num_vertices, std::vector<std::pair<int, double>>());

        long long max_arestas_extras = ((long long)num_vertices * (num_vertices - 1) / 2) - (num_vertices - 1);
        long long arestas_para_adicionar = (max_arestas_extras * densidade_percentual) / 100;
        long long total_arestas = (num_vertices - 1) + arestas_para_adicionar;

        arestas.reserve(total_arestas);
        int grau_estimado = static_cast<int>((2 * total_arestas) / num_vertices) + 10;
        for (int i = 0; i < num_vertices; ++i) {
            adj[i].reserve(grau_estimado);
        }

        std::uniform_real_distribution<double> dist_peso(1.0, 100.0);

        for (int i = 0; i < num_vertices - 1; ++i) {
            double peso = dist_peso(rng);
            arestas.push_back({i, i + 1, peso});
            adj[i].push_back({i + 1, peso});
            adj[i + 1].push_back({i, peso});
        }

        std::uniform_int_distribution<int> dist_vertice(0, num_vertices - 1);
        long long adicionadas = 0;
        long long tentativas_max = arestas_para_adicionar * 3;

        while (adicionadas < arestas_para_adicionar && tentativas_max-- > 0) {
            int u = dist_vertice(rng);
            int v = dist_vertice(rng);

            if (u != v) {
                if (u > v) std::swap(u, v);

                double peso = dist_peso(rng);
                arestas.push_back({u, v, peso});
                adj[u].push_back({v, peso});
                adj[v].push_back({u, peso});
                adicionadas++;
            }
        }
    }

}