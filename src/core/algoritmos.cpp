#include "algoritmos.h"
#include <queue>
#include <algorithm>
#include <limits>

namespace Core {

    std::vector<Aresta> algoritmoKruskal(int num_vertices, std::vector<Aresta>& arestas) {
        std::vector<Aresta> mst;
        mst.reserve(num_vertices - 1);

        std::sort(arestas.begin(), arestas.end());

        UnionFind uf(num_vertices);

        for (const auto& aresta : arestas) {
            if (uf.encontrar(aresta.u) != uf.encontrar(aresta.v)) {
                uf.unir(aresta.u, aresta.v);
                mst.push_back(aresta);

                if (mst.size() == static_cast<size_t>(num_vertices - 1)) {
                    break;
                }
            }
        }

        return mst;
    }

    struct ElementoHeap {
        int v;
        int pai;
        double chave;

        bool operator>(const ElementoHeap& outro) const {
            return chave > outro.chave;
        }
    };

    std::vector<Aresta> algoritmoPrim(int num_vertices, const std::vector<std::vector<std::pair<int, double>>>& adj) {
        std::vector<Aresta> mst;
        mst.reserve(num_vertices - 1);

        std::vector<double> chave(num_vertices, std::numeric_limits<double>::infinity());
        std::vector<int> pai(num_vertices, -1);
        std::vector<bool> dentro_da_mst(num_vertices, false);

        std::priority_queue<ElementoHeap, std::vector<ElementoHeap>, std::greater<ElementoHeap>> min_heap;

        int raiz = 0;
        chave[raiz] = 0.0;
        min_heap.push({raiz, -1, 0.0});

        while (!min_heap.empty()) {
            ElementoHeap atual = min_heap.top();
            min_heap.pop();

            int u = atual.v;

            if (dentro_da_mst[u]) continue;

            dentro_da_mst[u] = true;

            if (atual.pai != -1) {
                mst.push_back({atual.pai, u, atual.chave});
                if (mst.size() == static_cast<size_t>(num_vertices - 1)) {
                    break;
                }
            }

            for (const auto& vizinho : adj[u]) {
                int v = vizinho.first;
                double peso_uv = vizinho.second;

                if (!dentro_da_mst[v] && peso_uv < chave[v]) {
                    chave[v] = peso_uv;
                    pai[v] = u;
                    min_heap.push({v, u, chave[v]});
                }
            }
        }

        return mst;
    }

}