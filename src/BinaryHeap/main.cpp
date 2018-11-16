#include <iostream>
#include "BinaryHeap.h"
#include <vector>
int main() {
  BinaryHeap<int> a;
  std::vector<BinaryHeap<int>::Pointer> ptr;
  int in;
  for (int i = 0; i < 10; ++i) {
    in = 10 + i;
    ptr.push_back(a.insert(in));
    std::cout << in << ' ';
  }
  std::cout << '\n';
  for (int i = 0; i < 5; ++i) {
    std::cout << a.erase(ptr[2 * i]) << ' ';
  }
  std::cout << '\n';
  for (int i = 0; i < 5; ++i) {
    std::cout << a.extractMin() << ' ';
  }
}