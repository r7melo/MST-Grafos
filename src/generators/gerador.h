#ifndef GERADOR_H
#define GERADOR_H

#include "core/estruturas.h"
#include <vector>
#include <string>
#include <random>

namespace Generators {

    class Gerador {
    public:
        static void gerarGrafoNormal(
            int num_vertices, int densidade_percentual, 
            std::vector<Core::Aresta>& arestas, 
            std::vector<std::vector<std::pair<int, double>>>& adj,
            std::mt19937& rng
        );

        static void gerarGrafoGeometrico(
            int num_vertices, int densidade_percentual, 
            std::vector<Core::Aresta>& arestas, 
            std::vector<std::vector<std::pair<int, double>>>& adj,
            std::mt19937& rng
        );
    };

}

#endif