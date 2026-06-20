#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../heads/BinaryHeap.h"

using namespace std;

int main() {
    srand(time(nullptr));

    BinaryHeap<int> bh;

    for (int i = 0; i < 5; i++) {
        int rnum = (rand() % 100) + 1;
        cout << "-> " << rnum << endl;
        bh.insert(rnum);
    }

    while (!bh.isEmpty()) {
        cout << bh.extractMin() << " <-\n";
    }

    return 0;
}