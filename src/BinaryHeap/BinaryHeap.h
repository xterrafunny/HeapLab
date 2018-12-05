#include <vector>
#include <memory>
#include <iostream>

template<typename Key>
class BinaryHeap {
 public:

  class Pointer;
  BinaryHeap();
  template<class Iterator>
  BinaryHeap(Iterator begin, Iterator end);
  ~BinaryHeap();

  bool isEmpty();
  BinaryHeap<Key>::Pointer insert(Key value);
  Key getMin();
  Key extractMin();
  Key erase(BinaryHeap<Key>::Pointer pointer);
  void change(Pointer pointer, Key value);
  size_t size();
  void clear();
 private:

  struct Node {
    int index;
    std::weak_ptr<Node> ptr;
    Key value;
    std::shared_ptr<std::weak_ptr<Node>> ptr_to_ptr;
  };

  size_t size_;
  std::vector<std::shared_ptr<Node>> heap_;

  void siftDown_(Node node);
  void siftUp_(Node node);
  Key erase_(Node node);
  void swap_(Node& first, Node& second);
  std::shared_ptr<Node> newNode_(Key value);
};

template<typename Key>
class BinaryHeap<Key>::Pointer {
 public:
  Pointer() {}
  ~Pointer() {}

 private:
  friend class BinaryHeap<Key>;
  std::shared_ptr<std::weak_ptr<BinaryHeap<Key>::Node>> ptr;
};

template<typename Key>
BinaryHeap<Key>::BinaryHeap() {
  size_ = 0;
}

template<typename Key>
bool BinaryHeap<Key>::isEmpty() {
  return size_ == 0;
}

template<typename Key>
void BinaryHeap<Key>::siftUp_(BinaryHeap<Key>::Node node) {
  int index = node.index, parent_index = 0;
  Key node_value = node.value;
  while (index > 0) {
    parent_index = (index - 1) >> 1;
    if ((heap_[parent_index])->value < node_value) {
      return;
    }
    swap_(*(heap_[index]->ptr.lock()), *(heap_[parent_index]->ptr.lock()));
    index = parent_index;
  }
  return;
}

template<typename Key>
void BinaryHeap<Key>::swap_(BinaryHeap<Key>::Node& first, BinaryHeap<Key>::Node& second) {
  std::swap(first, second);
  std::swap(first.ptr_to_ptr, second.ptr_to_ptr);
  std::swap(first.value, second.value);
  std::swap(first.ptr, second.ptr);
  std::swap(heap_[first.index], heap_[second.index]);
}

template<typename Key>
std::shared_ptr<typename BinaryHeap<Key>::Node> BinaryHeap<Key>::newNode_(Key value) {
  Node new_node;
  auto inner_ptr_to_node = std::make_shared<BinaryHeap<Key>::Node>(new_node);
  auto inner_ptr_to_ptr = std::make_shared<std::shared_ptr<BinaryHeap<Key>::Node>>(inner_ptr_to_node);
  heap_.push_back(inner_ptr_to_node);
  inner_ptr_to_node->index = static_cast<int>(size_);
  ++size_;
  inner_ptr_to_node->value = value;
  inner_ptr_to_node->ptr = inner_ptr_to_node;
  inner_ptr_to_node->ptr_to_ptr = std::make_shared<std::weak_ptr<Node>>(inner_ptr_to_node);
  return inner_ptr_to_node;
}

template<typename Key>
typename BinaryHeap<Key>::Pointer BinaryHeap<Key>::insert(Key value) {
  std::shared_ptr<Node> new_node = newNode_(value);
  siftUp_(*new_node);
  Pointer return_ptr;
  return_ptr.ptr = std::make_shared<std::weak_ptr<Node>>(new_node->ptr);
  return return_ptr;
}

template<typename Key>
Key BinaryHeap<Key>::erase_(Node node) {
  int index = node.index;
  swap_(*(node.ptr.lock()), *((heap_.back())->ptr.lock()));
  Key return_val = heap_.back()->value;
  node = *(heap_.back());
  node.ptr_to_ptr.reset();
  node.ptr.reset();
  heap_.pop_back();
  size_--;
  if (index < size_) {
    siftDown_(*(heap_[index]->ptr.lock()));
  }
  return return_val;
}

template<typename Key>
void BinaryHeap<Key>::siftDown_(Node node) {
  int index = node.index, min_child_index = 0;
  while (2 * index + 1 < size_) {
    min_child_index = 2 * index + 1;
    if (2 * index + 2 < size_ && heap_[2 * index + 2]->value < heap_[2 * index + 1]->value) {
      ++min_child_index;
    }
    if (heap_[min_child_index]->value > node.value) {
      return;
    }
    swap_(*(node.ptr.lock()), *(heap_[min_child_index]->ptr.lock()));
    index = min_child_index;
  }
}

template<typename Key>
Key BinaryHeap<Key>::getMin() {
  return heap_.front()->value;
}

template<typename Key>
Key BinaryHeap<Key>::extractMin() {
  return erase_(*((heap_.front())->ptr.lock()));
}

template<typename Key>
Key BinaryHeap<Key>::erase(BinaryHeap<Key>::Pointer pointer) {
  Node node = *((*pointer.ptr).lock());
  return erase_(node);
}

template<typename Key>
template<class Iterator>
BinaryHeap<Key>::BinaryHeap(Iterator begin, Iterator end) {
  size_ = 0;
  for (Iterator i = begin; i != end; ++i) {
    newNode_(*i);
  }
  for (int i = (static_cast<int>(size_) + 1) / 2; i >= 0; i--) {
    siftDown_(*(heap_[i]->ptr));
  }
}

template<typename Key>
void BinaryHeap<Key>::change(BinaryHeap<Key>::Pointer pointer, Key value) {
  std::shared_ptr<Node> ptr_to_node = (*pointer.ptr).lock();
  if (ptr_to_node->value == value) {
    return;
  }
  if (ptr_to_node->value < value) {
    ptr_to_node->value = value;
    siftDown_(*ptr_to_node);
    return;
  }
  ptr_to_node->value = value;
  siftUp_(*ptr_to_node);
}

template<typename Key>
size_t BinaryHeap<Key>::size() {
  return size_;
}

template<typename Key>
void BinaryHeap<Key>::clear() {
  for (auto i = heap_.begin(); i != heap_.end(); ++i) {
    (*i)->ptr_to_ptr.reset();
    (*i)->ptr.reset();
  }
  size_ = 0;
  heap_.clear();
}

template<typename Key>
BinaryHeap<Key>::~BinaryHeap() {
  clear();
}