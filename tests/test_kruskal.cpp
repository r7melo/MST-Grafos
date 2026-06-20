#include <iostream>
#include <vector>
#include "../heads/Kruskal.h"

using namespace std;

int main() {
    // Lista de arestas
    // Uma aresta contém origem, destino e peso.
    vector<Edge> edges = {
        {0, 1, 10}, {1, 3, 15}, {2, 3, 4}, {2, 0, 6}, {0, 3, 5}
    };
    
    cout << kruskalsMST(4, edges);
    
    return 0;
}