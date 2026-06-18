#include "estruturas.h"

namespace Core {

    UnionFind::UnionFind(int n) {
        pai.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            pai[i] = i;
        }
    }

    int UnionFind::encontrar(int v) {
        if (v != pai[v]) {
            pai[v] = encontrar(pai[v]);
        }
        return pai[v];
    }

    void UnionFind::unir(int u, int v) {
        int raiz_u = encontrar(u);
        int raiz_v = encontrar(v);

        if (raiz_u != raiz_v) {
            if (rank[raiz_u] < rank[raiz_v]) {
                pai[raiz_u] = raiz_v;
            } else if (rank[raiz_u] > rank[raiz_v]) {
                pai[raiz_v] = raiz_u;
            } else {
                pai[raiz_v] = raiz_u;
                rank[raiz_u]++;
            }
        }
    }

}