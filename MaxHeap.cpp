#include "MaxHeap.hpp"
#include <algorithm>

// Constructor
MaxHeap::MaxHeap() {}

// Restore heap property by moving the element down
void MaxHeap::heapifyDown(int i)
{
    int largest = i;
    int l = left(i);
    int r = right(i);

    if (l < heap.size() && std::get<0>(heap[l]) > std::get<0>(heap[largest]))
    {
        largest = l;
    }
    if (r < heap.size() && std::get<0>(heap[r]) > std::get<0>(heap[largest]))
    {
        largest = r;
    }

    if (largest != i)
    {
        std::swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

// Restore heap property by moving the element up
void MaxHeap::heapifyUp(int i)
{
    while (i > 0 && std::get<0>(heap[parent(i)]) < std::get<0>(heap[i]))
    {
        std::swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}

// Insert a new element into the heap
void MaxHeap::insert(double weight, int currentNode, int parentNode)
{
    heap.emplace_back(weight, currentNode, parentNode);
    heapifyUp(heap.size() - 1);
}

// Extract the maximum element from the heap
std::tuple<double, int, int> MaxHeap::extractMax()
{
    if (heap.empty())
    {
        throw std::runtime_error("Heap is empty");
    }

    auto maxElement = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);

    return maxElement;
}

// Check if the heap is empty
bool MaxHeap::empty() const
{
    return heap.empty();
}
