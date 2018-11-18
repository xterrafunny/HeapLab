#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinaryHeap

#include "../src/BinaryHeap/BinaryHeap.h"
#include <gtest/gtest.h>

//TEST(LinkedListContructor, EmptySize) {
//  LinkedList<int> list = LinkedList<int> ();
//  EXPECT_EQ(list.size(), 0);
//}
//
//TEST(LinkedListContructor, VectorConstructor) {
//  std::vector<int> nums = {1, 2, 3};
//  LinkedList<int> list = LinkedList<int>(nums);
//
//  EXPECT_EQ(list.size(), nums.size());
//  EXPECT_EQ(list.NthElement(0), 1);
//  EXPECT_EQ(list.NthElement(1), 2);
//  EXPECT_EQ(list.NthElement(2), 3);
//}
//
//TEST(LinkedListBasic, PushBack) {
//  LinkedList<int> list = LinkedList<int>();
//  list.PushBack(1);
//  list.PushBack(2);
//  list.PushBack(3);
//
//  EXPECT_EQ(list.size(), 3);
//
//  EXPECT_EQ(list.NthElement(0), 1);
//  EXPECT_EQ(list.NthElement(1), 2);
//  EXPECT_EQ(list.NthElement(2), 3);
//}
//
//TEST(LinkedListBasic, PushFront) {
//  LinkedList<int> list = LinkedList<int>();
//  list.PushFront(1);
//  list.PushFront(2);
//  list.PushFront(3);
//
//  EXPECT_EQ(list.size(), 3);
//
//  EXPECT_EQ(list.NthElement(0), 3);
//  EXPECT_EQ(list.NthElement(1), 2);
//  EXPECT_EQ(list.NthElement(2), 1);
//}
//
//TEST(LinkedListReverse, Recursive) {
//  std::vector<int> nums = {1, 2, 3, 4, 5};
//  LinkedList<int> list = LinkedList<int>(nums);
//
//  list.ReverseRecursive();
//  std::reverse(nums.begin(), nums.end());
//
//  for (int i = 0; i < list.size(); ++i) {
//    EXPECT_EQ(list.NthElement(i), nums[i]);
//  }
//}
//
//TEST(LinkedListReverse, DivideAndConquer) {
//  std::vector<int> nums = {1, 2, 3, 4, 5};
//  LinkedList<int> list = LinkedList<int>(nums);
//
//  list.ReverseDivideAndConquer();
//  std::reverse(nums.begin(), nums.end());
//
//  for (int i = 0; i < list.size(); ++i) {
//    EXPECT_EQ(list.NthElement(i), nums[i]);
//  }
//}
TEST(BinaryHeapConstructor,Empty) {

}