#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinomialHeap

#include "FibHeap.h"
#include <gtest/gtest.h>
#include <random>
#include <algorithm>

TEST(FibHeapConstructor, EmptySize) {
  FibHeap<int> heap;
  EXPECT_EQ(heap.size(), 0);
}
TEST(FibHeapConstructor, N_Elements) {
  for (int n = 0; n < 100; ++n) {
    FibHeap<int> heap;
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    EXPECT_EQ(heap.size(), n);
  }
}

TEST(FibHeapBasic, Clear) {
  FibHeap<int> heap;
  for (int n = 0; n < 100; ++n) {
    for (int i = 0; i < n; ++i) {
      heap.insert(0);
    }
    heap.clear();
    EXPECT_EQ(heap.size(), 0);
    EXPECT_EQ(heap.isEmpty(), true);
  }
}

TEST(FibHeapBasic, GetMin) {
  FibHeap<unsigned long long> heap;
  std::mt19937_64 gen;
  unsigned long long input = 0, minElement = 18446744073709551615;
  for (int i = 0; i < 10000; ++i) {
    input = gen();
    heap.insert(input);
    minElement = std::min(minElement, input);
  }
  EXPECT_EQ(heap.getMin(), minElement);
}

TEST(FibHeapBasic, ExtractMin) {
  std::vector<unsigned long long> sorted, check;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int count = 0; count < 100; ++count) {
    for (int n = 1; n < 100; ++n) {
      FibHeap<unsigned long long> heap;
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

TEST(FibHeapBasic, Decrease) {
  std::vector<unsigned long long> sorted, check;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int count = 0; count < 100; ++count) {
    for (int n = 1; n < 100; ++n) {
      FibHeap<unsigned long long> heap;
      std::vector<FibHeap<unsigned long long>::Pointer> ptr(n);
      for (int i = 0; i < n; ++i) {
        input = gen();
        sorted.push_back(input);
        ptr[i] = heap.insert(input);
      }
      for (int i = 0; i < 100; ++i) {
        int pos = rand() % n;
        unsigned long long decrease = gen() % sorted[pos];
        heap.decrease(ptr[pos], sorted[pos] - decrease);
        sorted[pos] -= decrease;
      }
      std::sort(sorted.begin(), sorted.end());
      for (int i = 0; i < n; ++i) {
        check.push_back(heap.extractMin());
      }
      EXPECT_EQ(check, sorted);
      sorted.clear();
      check.clear();
      heap.clear();
    }
  }
}

TEST(FibHeapBasic, Erase) {
  std::vector<unsigned long long> sorted, check;
  std::vector<FibHeap<unsigned long long>::Pointer> ptr;
  std::mt19937_64 gen;
  unsigned long long input = 0;
  for (int count = 0; count < 3; ++count) {
    for (int n = 1; n < 100; ++n) {
      sorted.resize(n);
      ptr.resize(n);
      FibHeap<unsigned long long> heap;
      for (int i = 0; i < n; ++i) {
        sorted[i] = gen();
        ptr[i] = heap.insert(sorted[i]);
      }
      for (int i = 0; i * 2 < n; ++i) {
        int index = gen() % sorted.size();
        sorted.erase(sorted.begin() + index);
        heap.erase(ptr[index]);
        ptr.erase(ptr.begin() + index);
      }
      std::sort(sorted.begin(), sorted.end());
      for (int i = 0; i < sorted.size(); ++i) {
        check.push_back(heap.extractMin());
      }
      EXPECT_EQ(check, sorted);
      heap.clear();
      ptr.clear();
      sorted.clear();
      check.clear();
    }
  }
}