#include <vector>
#include <memory>

template<typename Key>
class BinHeap {
 public:

  class Pointer;
  BinHeap();
  ~BinHeap() = default;

  bool isEmpty();
  BinHeap<Key>::Pointer insert(Key value);
  Key getMin();
  Key extractMin();
  Key erase(BinHeap<Key>::Pointer pointer);
 private:

  struct Node {
    int index;
    std::shared_ptr<Node> ptr;
    Key value;
    std::weak_ptr<std::shared_ptr<Node>> ptr_to_ptr;
  };

  size_t size_;
  std::vector<std::shared_ptr<Node>> heap_;

  void siftDown_(Node node);
  void siftUp_(Node node);
  Key erase_(Node node);
  void swap_(Node& first, Node& second);
};

template<typename Key>
class BinHeap<Key>::Pointer {
 public:
  Pointer() {}
  ~Pointer() {}

 private:
  friend class BinHeap<Key>;
  std::shared_ptr<std::shared_ptr<BinHeap<Key>::Node>> ptr;
};

template<typename Key>
BinHeap<Key>::BinHeap() {
  size_ = 0;
}

template<typename Key>
bool BinHeap<Key>::isEmpty() {
  return size_ == 0;
}

template<typename Key>
void BinHeap<Key>::siftUp_(BinHeap<Key>::Node node) {
  int index = node.index, parent_index = 0;
  Key node_value = node.value;
  while (index > 0) {
    parent_index = (index - 1) >> 1;
    if ((heap_[parent_index])->value < node_value) {
      return;
    }
    swap_(*(heap_[index]->ptr), *(heap_[parent_index]->ptr));
    index = parent_index;
  }
  return;
}

template<typename Key>
void BinHeap<Key>::swap_(BinHeap<Key>::Node& first, BinHeap<Key>::Node& second) {
  std::swap(first, second);
  std::swap(first.ptr_to_ptr, second.ptr_to_ptr);
  std::swap(first.value, second.value);
  std::swap(first.ptr, second.ptr);
  std::swap(heap_[first.index], heap_[second.index]);
}

template<typename Key>
typename BinHeap<Key>::Pointer BinHeap<Key>::insert(Key value) {
  Node new_node;
  auto inner_ptr_to_node = std::make_shared<BinHeap<Key>::Node>(new_node);
  auto inner_ptr_to_ptr = std::make_shared<std::shared_ptr<BinHeap<Key>::Node>>(inner_ptr_to_node);
  heap_.push_back(inner_ptr_to_node);
  inner_ptr_to_node->index = static_cast<int>(size_);
  ++size_;
  inner_ptr_to_node->value = value;
  inner_ptr_to_node->ptr = inner_ptr_to_node;
  inner_ptr_to_node->ptr_to_ptr = inner_ptr_to_ptr;
  siftUp_(*inner_ptr_to_node);
  Pointer return_ptr;
  return_ptr.ptr = inner_ptr_to_ptr;
  return return_ptr;
}

template<typename Key>
Key BinHeap<Key>::erase_(Node node) {
  int index = node.index;
  swap_(*(node.ptr), *((heap_.back())->ptr));
  Key return_val = heap_.back()->value;
  heap_.pop_back();
  size_--;
  siftDown_(*(heap_[index]->ptr));
  return return_val;
}

template<typename Key>
void BinHeap<Key>::siftDown_(Node node) {
  int index = node.index, min_child_index = 0;
  while (2 * index + 1 < size_) {
    min_child_index = 2 * index + 1;
    if (2 * index + 2 < size_ && heap_[2 * index + 2]->value < heap_[2 * index + 1]->value) {
      ++min_child_index;
    }
    if (heap_[min_child_index]->value > node.value) {
      return;
    }
    swap_(*(node.ptr), *(heap_[min_child_index]->ptr));
    index = min_child_index;
  }
}

template<typename Key>
Key BinHeap<Key>::getMin() {
  return heap_.front()->value;
}

template<typename Key>
Key BinHeap<Key>::extractMin() {
  return erase_(*(heap_.front()->ptr));
}

template<typename Key>
Key BinHeap<Key>::erase(BinHeap<Key>::Pointer pointer) {
  return erase_(*(*(pointer.ptr))->ptr);
}