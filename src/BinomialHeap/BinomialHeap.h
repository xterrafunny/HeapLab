#include <list>
#include <memory>
#include <exception>

template<typename Key>
class BinomialHeap {
 public:
  BinomialHeap();
  ~BinomialHeap();
  bool isEmpty();
  size_t size();
  Key getMin();
  void insert(Key key);
  Key extractMin();
  void clear();
 private:
  struct Node {
    std::list<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;
    Key key;
    int degree;
    std::shared_ptr<Node> ptr_to_node;
    std::weak_ptr<std::shared_ptr<Node>> ptr_to_ptr;
  };
  typename std::list<std::shared_ptr<Node>>::iterator iterator_to_min_;
  std::list<std::shared_ptr<Node>> roots_;
  size_t size_;
  void clearNode(std::shared_ptr<Node> node);
  void merge_(BinomialHeap<Key> &heap);
  void nodeMerge_(std::shared_ptr<Node> &first,std::shared_ptr<Node> &second);
};

template<typename Key>
BinomialHeap<Key>::BinomialHeap() {
  size_ = 0;
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
  if (size_ == 0) {
    throw std::out_of_range("Empty heap");
  }
  return (*iterator_to_min_)->key;
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
  size_ += heap.size_;
  heap.size_ = 0;
  heap.roots_.clear();
  roots_ = new_roots;
  if (size_ == 0) {
    return;
  }
  iterator_to_min_ = roots_.begin();
  Key new_min = roots_.front()->key;
  for (auto i = roots_.begin(); i != roots_.end(); ++i) {
    if ((*i)->key < new_min) {
      new_min = (*i)->key;
      iterator_to_min_ = i;
    }
  }
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
  new_heap.size_ = 1;

  merge_(new_heap);
}

template<typename Key>
Key BinomialHeap<Key>::extractMin() {
  if (size_ == 0) {
    throw std::out_of_range("Empty heap");
  }
  BinomialHeap<Key> new_heap;
  Key return_value = getMin();
  new_heap.roots_ = (*iterator_to_min_)->children;
  (*iterator_to_min_)->ptr_to_ptr.reset();
  (*iterator_to_min_)->ptr_to_node.reset();
  for (auto i = new_heap.roots_.begin(); i != new_heap.roots_.end(); ++i) {
    (*i)->parent.reset();
  }
  new_heap.size_ = static_cast<size_t>(std::max((1 << ((*iterator_to_min_)->degree)) - 1, 0));
  size_ -= 1 + new_heap.size();
  roots_.erase(iterator_to_min_);
  merge_(new_heap);
  return return_value;
}

template<typename Key>
BinomialHeap<Key>::~BinomialHeap() {
  clear();
}

template<typename Key>
void BinomialHeap<Key>::clearNode(std::shared_ptr<BinomialHeap<Key>::Node> node) {
  node->parent.reset();
  node->ptr_to_ptr.reset();
  for (auto i = node->children.begin(); i != node->children.end(); ++i) {
    clearNode(*i);
  }
  node->ptr_to_node.reset();
}

template<typename Key>
void BinomialHeap<Key>::clear() {
  for (auto i = roots_.begin(); i != roots_.end(); ++i) {
    clearNode(*i);
  }
  size_ = 0;
}