#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class BinaryHeap {

private:
    vector<T> heap;

    bool isMinHeap; 
    int currentSize; 
    int maxCapacity;

    int parent(int index) { return (index - 1) / 2; }
    int leftChild(int index) { return (2 * index + 1); }
    int rightChild(int index) { return (2 * index + 2); }

    void heapifyUp(int index) {
        while (index && heap[index] < heap[parent(index)]) {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        size_t left = leftChild(index);
        size_t right = rightChild(index);
        size_t smallest = index;

        if (left < heap.size() && heap[left] < heap[smallest])
            smallest = left;

        if (right < heap.size() && heap[right] < heap[smallest])
            smallest = right;

        if (smallest != (size_t)index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:

    BinaryHeap() {
        isMinHeap = true;
        currentSize = 0;
        maxCapacity = -1;
    }

    bool isEmpty() {
        return heap.empty();
    }

    void insert(T key) {
        heap.push_back(key);
        currentSize = heap.size();
        heapifyUp(heap.size() - 1);
    }

    T extractMin() {
        if (isEmpty()) {
            throw runtime_error("Heap is empty");
        }

        T root = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        currentSize = heap.size();

        if (!isEmpty()) {
            heapifyDown(0);
        }
        return root;
    }
};

#endif // BINARY_HEAP_H