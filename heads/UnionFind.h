#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>
using namespace std;

class UnionFind {

private:
    vector<int> parent;

public:
    UnionFind(int size) {
      
        parent.resize(size);
        
        // Inicialize o array pai com cada 
        // elemento como seu próprio representante
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    int find(int i) {
        
        // Se i for raiz ou representante
        if (parent[i] == i) {
            return i;
        }
        
        // Caso contrário, faz a compressão de caminho
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
      
        // Representante do conjunto que contém i
        int irep = find(i);
        
        // Representante do conjunto que contém j
        int jrep = find(j);
        
        // Faça com que o representante do conjunto de i 
        // seja o representante do conjunto de j
        parent[irep] = jrep;
    }
    
};

#endif // UNION_FIND_H