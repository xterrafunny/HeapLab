#include "BinomialHeap.h"
#include <iostream>
using namespace std;
int main() {
  BinomialHeap<int> heap;
  for (int i = 0; i < 10; ++i) {
    heap.insert(10 + i);
  }

  for (int i = 0; i < 10; ++i) {
    cout << heap.extractMin() << ' ';
  }
  cout << "\nfine";
}