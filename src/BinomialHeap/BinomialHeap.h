#include <list>
#include <memory>

template<typename Key>
class BinomialHeap {
 public:
  BinomialHeap();
  ~BinomialHeap() = default;
  bool isEmpty();
  size_t size();
  Key getMin();
  void insert(Key key);
 private:
  struct Node {
    std::list<std::shared_ptr<Node>> children;
    std::weak_ptr<Node> parent;
    Key key;
    int degree;
    std::shared_ptr<Node> ptr_to_node;
    std::weak_ptr<std::shared_ptr<Node>> ptr_to_ptr;
  };
  std::shared_ptr<Node> ptr_to_min_;
  std::list<std::shared_ptr<Node>> roots_;
  size_t size_;
  void merge_(BinomialHeap<Key> &heap);
  void nodeMerge_(std::shared_ptr<Node> &first,std::shared_ptr<Node> &second);
};

template<typename Key>
BinomialHeap<Key>::BinomialHeap() {
  ptr_to_min_ = nullptr;
}

template<typename Key>
bool BinomialHeap<Key>::isEmpty() {
  return size_ == 0;
}

template<typename Key>
size_t BinomialHeap<Key>::size() {
  return size_;
}

template<typename Key>
Key BinomialHeap<Key>::getMin() {
  return ptr_to_min_->key;
}

template<typename Key>
void BinomialHeap<Key>::merge_(BinomialHeap<Key> &heap) {
  std::list<std::shared_ptr<BinomialHeap<Key>::Node>> new_roots;
  auto this_heap_iterator = roots_.begin(), other_heap_iterator = heap.roots_.begin();
  std::shared_ptr<Node> add_node;
  while (this_heap_iterator != roots_.end() || other_heap_iterator != heap.roots_.end()) {
    if (this_heap_iterator == roots_.end()) {
      add_node = (*(other_heap_iterator++));
    } else if (other_heap_iterator == heap.roots_.end()) {
      add_node = *(this_heap_iterator++);
    } else {
      if ((*this_heap_iterator)->degree < (*other_heap_iterator)->degree) {
        add_node = *(this_heap_iterator++);
      } else if ((*this_heap_iterator)->degree > (*other_heap_iterator)->degree) {
        add_node = *(other_heap_iterator++);
      } else {
        if ((*this_heap_iterator)->key < (*other_heap_iterator)->key) {
          nodeMerge_(*this_heap_iterator, *other_heap_iterator);
          add_node = *this_heap_iterator;
        } else {
          nodeMerge_(*other_heap_iterator, *this_heap_iterator);
          add_node = *other_heap_iterator;
        }
        ++this_heap_iterator;
        ++other_heap_iterator;
      }
    }
    if (!new_roots.empty() && add_node->degree == new_roots.back()->degree) {
      if (new_roots.back()->key < add_node->key) {
        nodeMerge_(new_roots.back(), add_node);
      } else {
        nodeMerge_(add_node, new_roots.back());
        new_roots.pop_back();
        new_roots.push_back(add_node);
      }
    } else {
      new_roots.push_back(add_node);
    }
  }
  if (size_ == 0) {
    ptr_to_min_ = heap.ptr_to_min_;
  }
  else if (!heap.isEmpty()) {
    ptr_to_min_ = (ptr_to_min_->key < (heap.ptr_to_min_)->key) ? ptr_to_min_ : heap.ptr_to_min_;
  }
  size_ += heap.size_;
  heap.size_ = 0;
  heap.roots_.clear();
  heap.ptr_to_min_ = nullptr;
  roots_ = new_roots;

}

template<typename Key>
void BinomialHeap<Key>::nodeMerge_(std::shared_ptr<BinomialHeap<Key>::Node> &first,
    std::shared_ptr<BinomialHeap<Key>::Node> &second) {
  second->parent = first;
  ++first->degree;
  first->children.push_back(second);
}

template<typename Key>
void BinomialHeap<Key>::insert(Key key) {
  Node new_node;
  new_node.ptr_to_node = std::make_shared<BinomialHeap<Key>::Node>(new_node);
  auto inner_ptr_to_node = std::make_shared<BinomialHeap<Key>::Node>(new_node);
  auto inner_ptr_to_ptr = std::make_shared<std::shared_ptr<BinomialHeap<Key>::Node>>(inner_ptr_to_node);
  inner_ptr_to_node->ptr_to_ptr = inner_ptr_to_ptr;
  inner_ptr_to_node->ptr_to_node = inner_ptr_to_node;
  inner_ptr_to_node->degree = 0;
  inner_ptr_to_node->key = key;
  inner_ptr_to_node->parent.reset();
  BinomialHeap<Key> new_heap;
  new_heap.roots_.push_back(inner_ptr_to_node);
  new_heap.ptr_to_min_ = inner_ptr_to_node;
  new_heap.size_ = 1;
  merge_(new_heap);
}