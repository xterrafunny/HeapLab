#include <iostream>
#include "BinaryHeap.h"
#include <vector>

int main() {
  BinaryHeap<int> a;
  std::vector<int> b;
  std::vector<BinaryHeap<int>::Pointer> ptr;
  int in;
  for (int i = 0; i < 10; ++i) {
    in = i + 10;
    ptr.push_back(a.insert(in));
    std::cout << in << ' ';
  }

//  std::cout << '\n';
//  for (int i = 0; i < 5; ++i) {
//    std::cout << a.erase(ptr[2 * i]) << ' ';
//  }
  a.change(ptr[0], 20);
  std::cout << '\n';
  for (int i = 0; i < 10; ++i) {
    std::cout << a.extractMin() << ' ';
  }
}