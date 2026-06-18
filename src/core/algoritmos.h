#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "estruturas.h"
#include <vector>

namespace Core {
    std::vector<Aresta> algoritmoKruskal(int num_vertices, std::vector<Aresta>& arestas);
    std::vector<Aresta> algoritmoPrim(int num_vertices, const std::vector<std::vector<std::pair<int, double>>>& adj);
}

#endif