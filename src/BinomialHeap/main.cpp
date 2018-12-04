#include "BinomialHeap.h"
#include <iostream>
#include <vector>
using namespace std;
int main() {
  BinomialHeap<int> heap;
  for (int i = 0; i < 10; ++i) {
    heap.insert(10 + i);
  }
  cout << "\nfine";
}