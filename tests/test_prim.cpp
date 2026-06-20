#include <iostream>
#include <vector>
#include "../heads/Prim.h"

using namespace std;

int main() {

    // Matriz de adjacência
    vector<vector<uint8_t>> graph = { 
        { 0, 2, 0, 6, 0 },
        { 2, 0, 3, 8, 5 },
        { 0, 3, 0, 0, 7 },
        { 6, 8, 0, 0, 9 },
        { 0, 5, 7, 9, 0 } 
    };

    // Execução do Prim
    vector<int> parent = primMST(graph);

    cout << "Custo Total (prim): " << calculateWeights(parent, graph);

    return 0;
}