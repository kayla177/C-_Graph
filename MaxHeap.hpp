#ifndef MAX_HEAP_HPP
#define MAX_HEAP_HPP

#include <vector>
#include <tuple>
#include <stdexcept>

class MaxHeap {
private:
    // store the heap elements as (weight, current node, parent node)
    std::vector<std::tuple<double, int, int>> heap;
    
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyDown(int i);
    void heapifyUp(int i);

public:
    MaxHeap();

    void insert(double weight, int currentNode, int parentNode);
    std::tuple<double, int, int> extractMax();
    bool empty() const;
};

#endif
