#include <iostream>
#include "../heads/UnionFind.h"

int main() {

    UnionFind uf(5);

    uf.unite(1, 2);
    uf.unite(3, 4);
    uf.unite(2, 0);

    bool inSameSet = (uf.find(1) == uf.find(4));
    cout << "Estao no mesmo conjunto? " << (inSameSet ? "Sim" : "Nao") << endl;


    uf.unite(0, 4);

    inSameSet = (uf.find(1) == uf.find(4));
    cout << "Estao no mesmo conjunto? " << (inSameSet ? "Sim" : "Nao") << endl;

    return 0;
}