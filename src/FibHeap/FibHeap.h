#include <vector>
#include <utility>
#include <memory>

template<typename Key>
class FibHeap {
 public:
  class Pointer;
  FibHeap();
  ~FibHeap();
  bool isEmpty();
  Pointer insert(Key key);
  Key getMin();
  Key extractMin();
  void merge(FibHeap<Key> &otherHeap);
  void decrease(Pointer ptr, Key key);
  size_t size();
  Key erase(Pointer ptr);
  void consolidate();
  void clear();
 private:
  size_t size_;
  struct Node {
    Key key;
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::weak_ptr<Node> child;
    int degree;
    bool mark;
  };
  int max_degree_;
  std::shared_ptr<Node> min_ptr_;
  void cut(std::shared_ptr<Node>& node);
  void cascadingCut(std::shared_ptr<Node> node);
};

template<typename Key>
class FibHeap<Key>::Pointer {
 private:
  friend class FibHeap;
  std::weak_ptr<FibHeap<Key>::Node> ptr;
};
template<typename Key>
FibHeap<Key>::FibHeap() {
  size_ = 0;
  max_degree_ = 0;
}

template<typename Key>
FibHeap<Key>::~FibHeap(){
  if (min_ptr_ != nullptr) {
    min_ptr_->right.reset();
  }
  min_ptr_ = nullptr;
}

template<typename Key>
bool FibHeap<Key>::isEmpty() {
  return size_ == 0;
}

template<typename Key>
size_t FibHeap<Key>::size() {
  return size_;
}

template<typename Key>
typename FibHeap<Key>::Pointer FibHeap<Key>::insert(Key key) {
  FibHeap<Key> new_heap;
  Pointer ptr;
  new_heap.size_ = 1;
  new_heap.max_degree_ = 0;
  Node* new_node = new Node;
  std::shared_ptr<Node> inner_ptr(new_node);
  inner_ptr->key = key;
  inner_ptr->left = inner_ptr->right = inner_ptr;
  inner_ptr->degree = 0;
  inner_ptr->mark = false;
  inner_ptr->parent.reset();
  inner_ptr->child.reset();
  new_heap.min_ptr_ = inner_ptr;
  merge(new_heap);
  ptr.ptr = inner_ptr;
  return ptr;
}

template<typename Key>
Key FibHeap<Key>::getMin() {
  return min_ptr_->key;
}

template<typename Key>
Key FibHeap<Key>::extractMin() {
  Key return_value = getMin();
  if (size_ == 1) {
    size_ = 0;
    min_ptr_->right.reset();
    min_ptr_ = nullptr;
  } else if (min_ptr_->child.expired()) {
    std::shared_ptr<Node> prev_root = min_ptr_->left.lock(), next_root = min_ptr_->right;
    prev_root->right = next_root;
    next_root->left = prev_root;
    min_ptr_ = next_root;
    consolidate();
    size_--;
  } else if (min_ptr_->right == min_ptr_) {
    min_ptr_->right.reset();
    min_ptr_ = min_ptr_->child.lock();
    consolidate();
    size_--;
  } else {
    std::shared_ptr<Node> next_root = min_ptr_->right, prev_root = min_ptr_->left.lock(), last_child = min_ptr_->child.lock()->left.lock();
    prev_root->right = min_ptr_->child.lock();
    min_ptr_->child.lock()->left = prev_root;
    next_root->left = last_child;
    last_child->right = next_root;
    min_ptr_ = next_root;
    consolidate();
    size_--;
  }
  return return_value;
}

template<typename Key>
void FibHeap<Key>::merge(FibHeap<Key> &otherHeap) {
  if (min_ptr_ == nullptr) {
    min_ptr_ = otherHeap.min_ptr_;
    size_ = otherHeap.size_;
    max_degree_ = std::max(max_degree_, otherHeap.max_degree_);
    otherHeap.min_ptr_.reset();
    otherHeap.size_ = 0;
    return;
  }
  max_degree_ = std::max(max_degree_, otherHeap.max_degree_);
  size_ += otherHeap.size_;
  std::shared_ptr<Node> next_in_this = min_ptr_->right, prev_in_other = otherHeap.min_ptr_->left.lock();
  min_ptr_->right = otherHeap.min_ptr_;
  otherHeap.min_ptr_->left = min_ptr_;
  next_in_this->left = prev_in_other;
  prev_in_other->right = next_in_this;
  if (min_ptr_->key > otherHeap.min_ptr_->key) {
    min_ptr_ = min_ptr_->right;
  }
  otherHeap.size_ = 0;
  otherHeap.min_ptr_.reset();
}

template<typename Key>
void FibHeap<Key>::decrease(FibHeap::Pointer ptr, Key key) {
  std::shared_ptr<Node> node = ptr.ptr.lock();
  if (node->parent.expired()) {
    node->key = key;
    if (min_ptr_->key >= key) {
      min_ptr_ = node;
    }
  } else if (node->parent.lock()->key < key) {
    node->key = key;
  } else {
    node->key = key;
    cut(node);
    cascadingCut(node->parent.lock());
  }
}

template<typename Key>
void FibHeap<Key>::consolidate() {
  std::vector<std::shared_ptr<Node>> roots(max_degree_ + 1, nullptr);
  std::shared_ptr<Node> current_node = min_ptr_, next_node = min_ptr_->right;
  roots[min_ptr_->degree] = current_node;
  current_node = current_node->right;
  next_node = current_node->right;
  while (current_node != min_ptr_) {
    current_node->parent.reset();
    if (roots[current_node->degree] == nullptr) {
      roots[current_node->degree] = current_node;
      current_node = current_node->right;
    } else {
      next_node = current_node->right;
      while (roots[current_node->degree] != nullptr) {
        if (current_node->key < roots[current_node->degree]->key) {
          std::swap(current_node, roots[current_node->degree]);
        }
        current_node->parent = roots[current_node->degree];
        if (roots[current_node->degree]->child.expired()) {
          roots[current_node->degree]->child = current_node;
          current_node->right = current_node;
          current_node->left = current_node;
        } else {
          current_node->right = roots[current_node->degree]->child.lock()->right;
          current_node->left = roots[current_node->degree]->child;
          current_node->left.lock()->right = current_node;
          current_node->right->left = current_node;
        }
        roots[current_node->degree]->degree++;
        current_node = current_node->parent.lock();
        roots[current_node->degree - 1].reset();
        if (current_node->degree == roots.size()) {
          roots.push_back(nullptr);
        }
      }
      max_degree_ = std::max(max_degree_, current_node->degree);
      roots[current_node->degree] = current_node;
      current_node = next_node;
    }
  }
  current_node.reset();
  int min_degree = -1;
  for (int i = 0; i < roots.size(); ++i) {
    if (roots[i] == nullptr) {
      continue;
    }
    if (current_node == nullptr) {
      current_node = roots[i];
      min_degree = i;
      min_ptr_ = current_node;
    } else {
      if (roots[i]->key < min_ptr_->key) {
        min_ptr_ = roots[i];
      }
      roots[i]->left = current_node;
      current_node->right = roots[i];
      current_node = roots[i];
    }
  }
  current_node->right = roots[min_degree];
  roots[min_degree]->left = current_node;
}

template<typename Key>
void FibHeap<Key>::cascadingCut(std::shared_ptr<FibHeap::Node> node) {
  while (node != nullptr && node->mark) {
    cut(node);
    node = node->parent.lock();
  }
  node->mark = true;
}

template<typename Key>
void FibHeap<Key>::cut(std::shared_ptr<FibHeap::Node> &node) {
  std::shared_ptr<Node> prev_node = node->left.lock(), next_node = node->right;
  node->parent.lock()->degree--;
  if (next_node == node) {
    node->parent.lock()->child.reset();
  } else {
    next_node->left = prev_node;
    prev_node->right = next_node;
    if (node->parent.lock()->child.lock() == node) {
      node->parent.lock()->child = next_node;
    }
  }
  node->mark = false;
  node->parent.reset();
  min_ptr_->right->left = node;
  node->right = min_ptr_->right;
  node->left = min_ptr_;
  min_ptr_->right = node;
  if (min_ptr_->key >= node->key) {
    min_ptr_ = min_ptr_->right;
  }
}

template<typename Key>
Key FibHeap<Key>::erase(FibHeap<Key>::Pointer ptr) {
  decrease(ptr, min_ptr_->key);
  return extractMin();
}

template<typename Key>
void FibHeap<Key>::clear() {
  while (!isEmpty()) {
    extractMin();
  }
}