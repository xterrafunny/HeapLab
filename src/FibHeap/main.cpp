#include <bits/stdc++.h>
#include "FibHeap.h"
using namespace std;

int main() {
  FibHeap<int> heap;
  vector<FibHeap<int>::Pointer> ptr(10);
  for (int i = 0; i < 10; ++i) {
    ptr[i] = heap.insert(10 + i);
  }
  heap.erase(ptr[7]);
  for (int i = 0; i < 9; ++i) {
    cout << heap.extractMin() << ' ';
  }
  cout << "fine";
}