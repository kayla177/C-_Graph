#include "MaxHeap.hpp"
#include <algorithm>
#include <tuple>

// Constructor
MaxHeap::MaxHeap() {}

void MaxHeap::heapifyDown(int i)
{
    int largest = i;
    int l = left(i);
    int r = right(i);

    // if the left child exists and its weight is greater than the current largest
    if (l < heap.size() && std::get<0>(heap[l]) > std::get<0>(heap[largest]))
    {
        // update largest
        largest = l;
    }
    // if the right child exists and its weight is greater than the current largest
    if (r < heap.size() && std::get<0>(heap[r]) > std::get<0>(heap[largest]))
    {
        largest = r;
    }

    if (largest != i)
    {
        // if the largest if not the current element, swap them, and call heapify again
        std::swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

void MaxHeap::heapifyUp(int i)
{
    // if current element is not the root and its parent's weight is smaller
    while (i > 0 && std::get<0>(heap[parent(i)]) < std::get<0>(heap[i]))
    {
        std::swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}

void MaxHeap::insert(double weight, int currentNode, int parentNode)
{
    // add the new element to the end of the heap
    heap.emplace_back(weight, currentNode, parentNode);
    // call heapifyUp to restore the heap property
    heapifyUp(heap.size() - 1);
}

std::tuple<double, int, int> MaxHeap::extractMax()
{
    if (heap.empty())
    {
        throw std::runtime_error("Heap is empty");
    }

    // store the max element
    auto maxElement = heap[0];
    // replace the root withe the last element
    heap[0] = heap.back();
    heap.pop_back();
    // restore heap property
    heapifyDown(0);

    return maxElement;
}

// check if the heap is empty
bool MaxHeap::empty() const
{
    return heap.empty();
}
