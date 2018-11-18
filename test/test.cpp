#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinaryHeap

#include "../src/BinaryHeap/BinaryHeap.h"
#include <gtest/gtest.h>
#include <random>
#include <algorithm>

TEST(BinaryHeapConstructor, EmptySize) {
  BinaryHeap<int> heap;
  EXPECT_EQ(heap.size(), 0);
}
TEST(BinaryHeapConstructor, N_Elements) {
  BinaryHeap<int> heap;
  for (int n = 0; n < 1000; ++n) {
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    EXPECT_EQ(heap.size(), n);
    heap.clear();
  }
}
TEST(BinaryHeapBasic, Clear) {
  BinaryHeap<int> heap;
  for (int n = 0; n < 1000; ++n) {
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    heap.clear();
    EXPECT_EQ(heap.size(), 0);
    EXPECT_EQ(heap.isEmpty(), true);
  }
}

TEST(BinaryHeapBasic, GetMin) {
  BinaryHeap<unsigned long long> heap;
  std::mt19937_64 gen;
  unsigned long long input = 0, minElement = 18446744073709551615;
  for (int i = 0; i < 100000; ++i) {
    input = gen();
    heap.insert(input);
    minElement = std::min(minElement, input);
  }
  EXPECT_EQ(heap.getMin(), minElement);
}

TEST(BinaryHeapBasic, ExtractMin) {
  BinaryHeap<unsigned long long> heap;
  std::vector<unsigned long long> sorted, check;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int count = 0; count < 100; ++count) {
    for (int n = 1; n < 100; ++n) {
      for (int i = 0; i < n; ++i) {
        input = gen();
        sorted.push_back(input);
        heap.insert(input);
      }
      std::sort(sorted.begin(), sorted.end());
      for (int i = 0; i < n; ++i) {
        check.push_back(heap.extractMin());
      }
      EXPECT_EQ(sorted, check);
      sorted.clear();
      check.clear();
      heap.clear();
    }
  }
}

TEST(BinaryHeapBasic, EraseAll) {
  BinaryHeap<unsigned long long> heap;
  std::vector<BinaryHeap<unsigned long long>::Pointer> ptr;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int i = 0; i < 100000; ++i) {
    input = gen();
    ptr.push_back(heap.insert(input));
  }
  for (int i = 0; i < 100000; ++i) {
    heap.erase(ptr[i]);
    EXPECT_EQ(heap.size(), 99999 - i);
  }
  EXPECT_EQ(heap.isEmpty(), true);
}