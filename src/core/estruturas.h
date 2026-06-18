#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <vector>

namespace Core {

    struct Aresta {
        int u;
        int v;
        double peso;
        bool operator<(const Aresta& outra) const {return peso < outra.peso;}
    };

    struct UnionFind {
        std::vector<int> pai;
        std::vector<int> rank;
        UnionFind(int n);
        int encontrar(int v);
        void unir(int u, int v);
    };

}

#endif