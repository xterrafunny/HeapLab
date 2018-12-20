#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinomialHeap

#include "BinomialHeap.h"
#include <gtest/gtest.h>
#include <random>
#include <algorithm>

TEST(BinomialHeapConstructor, EmptySize) {
  BinomialHeap<int> heap;
  EXPECT_EQ(heap.size(), 0);
}
TEST(BinomialHeapConstructor, N_Elements) {
  for (int n = 0; n < 100; ++n) {
    BinomialHeap<int> heap;
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    EXPECT_EQ(heap.size(), n);
  }
}

TEST(BinomialHeapBasic, Clear) {
  BinomialHeap<int> heap;
  for (int n = 0; n < 100; ++n) {
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    heap.clear();
    EXPECT_EQ(heap.size(), 0);
    EXPECT_EQ(heap.isEmpty(), true);
  }
}

TEST(BinomialHeapBasic, GetMin) { BinomialHeap<unsigned long long> heap;
  std::mt19937_64 gen;
  unsigned long long input = 0, minElement = 18446744073709551615;
  for (int i = 0; i < 10000; ++i) {
    input = gen();
    heap.insert(input);
    minElement = std::min(minElement, input);
  }
  EXPECT_EQ(heap.getMin(), minElement);
}

TEST(BinomialHeapBasic, ExtractMin) {
  std::vector<unsigned long long> sorted, check;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int count = 0; count < 1; ++count) {
    for (int n = 1; n < 100; ++n) {
      BinomialHeap<unsigned long long> heap;
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