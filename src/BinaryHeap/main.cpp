#include <iostream>
#include "BinaryHeap.h"
#include <vector>

int main() {
  BinaryHeap<int> heap;
  std::vector<BinaryHeap<int>::Pointer> ptr(10);
  for (int i = 0; i < 10; ++i) {
    ptr[i] = heap.insert(i + 10);
  }
  for (int i = 0; i < 10; ++i) {
    heap.change(ptr[i], i + 2);
  }
  for (int i = 0; i < 10; ++i) {
    std::cout << heap.extractMin() << '\n';
  }
}