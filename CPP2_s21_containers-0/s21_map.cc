#ifndef SRC_S21_MAP_CC_
#define SRC_S21_MAP_CC_
#include "s21_map.h"

namespace s21 {
/*Конструкторы, деструкторы ************************************/

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::map() : size_(0) {
  CreateFake();
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::CreateFake() {
  fake_ = new Node({key_type(), mapped_type()});
  fake_->left_ = fake_->right_ = fake_;
  fake_->color_ = ColorNode::BLACK;
}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::map(std::initializer_list<value_type> const &items)
    : map() {
  for (const_reference val : items) {
    insert(val);
  }
}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::map(const map &m) : map() {
  if (this != &m) {
    if (m.size_) {
      Node *new_node = nullptr;
      Node *m_node = m.fake_->parent_;
      const Node *m_fake = m.fake_;
      fake_->parent_ = CopyAll(m_node, m_fake, new_node);
      size_ = m.size_;
    }
  }
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::Node *map<key_type, mapped_type>::CopyAll(
    Node *&m_node, const Node *&m_fake, Node *node) {
  if (!m_node) return nullptr;
  if (m_node == m_fake) return fake_;

  Node *parent = node;
  node = new Node(m_node->data_, m_node->color_);
  node->left_ = CopyAll(m_node->left_, m_fake, node);
  if (node->left_ == fake_) {
    fake_->left_ = node;
  }
  node->right_ = CopyAll(m_node->right_, m_fake, node);
  if (node->right_ == fake_) {
    fake_->right_ = node;
  }
  node->parent_ = parent;
  return node;
}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::map(map &&m) : map() {
  if (this != &m) swap(m);
}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::~map() {
  clear();
  delete fake_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::operator=(map &&m) {
  if (this != &m) {
    clear();
    swap(m);
  }
}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::Node::Node(value_type data, Node *parent)
    : data_{data},
      parent_{parent},
      left_{nullptr},
      right_{nullptr},
      color_{RED} {}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::Node::Node(value_type data, ColorNode color)
    : data_{data},
      parent_{nullptr},
      left_{nullptr},
      right_{nullptr},
      color_{color} {}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::MapIterator::MapIterator()
    : obj_{nullptr}, iter_{nullptr} {}

template <typename key_type, typename mapped_type>
map<key_type, mapped_type>::MapIterator::MapIterator(Node *&iter,
                                                     const map &obj)
    : obj_{&obj}, iter_{iter} {}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::MapIterator &
map<key_type, mapped_type>::iterator::operator=(const MapIterator &other) {
  this->iter_ = other.iter_;
  this->obj_ = other.obj_;
  return *this;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::reference
map<key_type, mapped_type>::iterator::operator*() {
  return iter_->data_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::value_type *
map<key_type, mapped_type>::iterator::operator->() {
  return &operator*();
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::MapIterator &
map<key_type, mapped_type>::iterator::operator++() {
  if (iter_->right_) {
    iter_ = iter_->right_;
    while (iter_ != obj_->fake_ && iter_->left_) iter_ = iter_->left_;
  } else {
    Node *right_child = nullptr;
    do {
      right_child = iter_;
      iter_ = iter_->parent_;
    } while (iter_->parent_ && iter_->right_ == right_child);
  }
  return *this;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::MapIterator
map<key_type, mapped_type>::iterator::operator++(int) {
  iterator tmp(*this);
  operator++();
  return tmp;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::MapIterator &
map<key_type, mapped_type>::iterator::operator--() {
  if (iter_ == obj_->fake_) {
    iter_ = obj_->fake_->right_;
  } else if (iter_->left_) {
    iter_ = iter_->left_;
    while (iter_ != obj_->fake_ && iter_->right_) {
      iter_ = iter_->right_;
    }
  } else {
    Node *left_child = nullptr;
    do {
      left_child = iter_;
      iter_ = iter_->parent_;
    } while (iter_->parent_ && iter_->left_ == left_child);
  }
  return *this;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::iterator::operator==(
    const MapIterator &other) const {
  return iter_ == other.iter_;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::iterator::operator!=(
    const MapIterator &other) const {
  return iter_ != other.iter_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::iterator
map<key_type, mapped_type>::begin() {
  if (!empty()) return iterator(fake_->left_, *this);
  return end();
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::iterator
map<key_type, mapped_type>::end() {
  return iterator(this->fake_, *this);
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::mapped_type &
map<key_type, mapped_type>::at(const key_type &key) {
  if (size_) {
    DirectionChild child = DirectionChild::LEFT_CHILD;
    std::pair<bool, Node *> res_pair =
        FindPosToInsert(Root(), std::make_pair(key, mapped_type()), child);
    if (!res_pair.first) return res_pair.second->GetValue();
  }
  throw std::out_of_range("Out of range");
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::mapped_type &
map<key_type, mapped_type>::operator[](const key_type &key) {
  return insert(std::make_pair(key, mapped_type())).first->second;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::empty() const {
  return !size_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::size_type
map<key_type, mapped_type>::size() const {
  return size_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::size_type
map<key_type, mapped_type>::max_size() const {
  return std::numeric_limits<int64_t>::max() / sizeof(Node);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::clear() {
  DeleteAll(fake_->parent_);
  size_ = 0;
  fake_->parent_ = fake_->left_ = fake_->right_ = nullptr;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::DeleteAll(Node *node) {
  if (!node || node == fake_) return;
  DeleteAll(node->left_);
  DeleteAll(node->right_);
  delete node;
}

template <typename key_type, typename mapped_type>
std::pair<typename map<key_type, mapped_type>::iterator, bool>
map<key_type, mapped_type>::insert(const_reference value) {
  return UniversalInsertOrAssign(value, false);
}

template <typename key_type, typename mapped_type>
std::pair<typename map<key_type, mapped_type>::iterator, bool>
map<key_type, mapped_type>::insert(const key_type &key,
                                   const mapped_type &obj) {
  return insert(std::make_pair(key, obj));
}

template <typename key_type, typename mapped_type>
std::pair<typename map<key_type, mapped_type>::iterator, bool>
map<key_type, mapped_type>::insert_or_assign(const key_type &key,
                                             const mapped_type &obj) {
  return UniversalInsertOrAssign(std::make_pair(key, obj), true);
}

template <typename key_type, typename mapped_type>
std::pair<bool, typename map<key_type, mapped_type>::Node *>
map<key_type, mapped_type>::FindPosToInsert(
    Node *node, const_reference value,
    map<key_type, mapped_type>::DirectionChild &child) {
  key_type key = node->GetKey();
  if (value.first == key) {
    return {false, node};
  }
  if (value.first < key) {
    if (!node->left_ || node->left_ == fake_) {
      return {true, node};
    }
    node = node->left_;
  } else {
    if (!node->right_ || node->right_ == fake_) {
      child = DirectionChild::RIGHT_CHILD;
      return {true, node};
    }
    node = node->right_;
  }
  return FindPosToInsert(node, value, child);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::InsertToBlank(Node *&new_node) {
  new_node->color_ = ColorNode::BLACK;
  new_node->left_ = new_node->right_ = fake_;
  Root() = fake_->left_ = fake_->right_ = new_node;
  ++size_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::InsertToNotBlank(Node *&node,
                                                  const DirectionChild &child,
                                                  const_reference value) {
  Node *new_node = new Node(value, node);
  if (child == DirectionChild::LEFT_CHILD) {
    if (node == fake_->left_) {
      fake_->left_ = new_node;
      new_node->left_ = fake_;
    }
    node->left_ = new_node;
  } else {
    if (node->right_ == fake_) {
      new_node->right_ = fake_;
      fake_->right_ = new_node;
    }
    node->right_ = new_node;
  }
  node = new_node;
  ++size_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::BalancingTree(Node *&node) {
  while (node->parent_ && node->parent_->color_ == ColorNode::RED) {
    Node *parent = node->parent_;
    Node *uncle = node->Uncle();
    Node *grandparent = node->GrandParent();
    if (!uncle || uncle == fake_ || uncle->color_ == ColorNode::BLACK) {
      if (parent->IsNodeRightChild()) {
        if (node->IsNodeLeftChild()) {
          node = node->parent_;
          RightRotate(node);
        }
        ChangeColorParentAndGrandparent(node);
        LeftRotate(node->parent_->parent_);
      } else {
        if (node->IsNodeRightChild()) {
          node = node->parent_;
          LeftRotate(node);
        }
        ChangeColorParentAndGrandparent(node);
        RightRotate(node->parent_->parent_);
      }
    } else {
      if (grandparent != Root()) grandparent->color_ = ColorNode::RED;
      parent->color_ = uncle->color_ = ColorNode::BLACK;
      node = grandparent;
    }
  }
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::ChangeColorParentAndGrandparent(Node *&node) {
  node->parent_->parent_->color_ = ColorNode::RED;
  node->parent_->color_ = ColorNode::BLACK;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::erase(iterator pos) {
  if (size_ == 1) {
    fake_->left_ = fake_->right_ = nullptr;
    delete fake_->parent_;
    fake_->parent_ = nullptr;
    size_ = 0;
    return;
  }
  Node *node = pos.iter_;
  if (!node || node == fake_)
    throw std::runtime_error("Node for erase is NULL");
  int mask_count_children = HowMuchChildren(node);
  if (mask_count_children == 0b10) {
    SwapNodes(node, node->left_);
    return;
  } else if (mask_count_children == 0b01) {
    SwapNodes(node, node->right_);
    return;
  } else if (mask_count_children == 0b11) {
    Node *next_node = (++pos).iter_;
    if (node == Root()) {
      fake_->parent_ = next_node;
    } else if (node->IsNodeLeftChild()) {
      node->parent_->left_ = next_node;
    } else {
      node->parent_->right_ = next_node;
    }
    if (next_node->left_ && next_node->left_ != fake_)
      next_node->left_->parent_ = node;

    if (next_node->right_ && next_node->right_ != fake_)
      next_node->right_->parent_ = node;

    if (node == next_node->right_) {
      node->right_ = next_node->right_;
      next_node->right_ = node;
      next_node->parent_ = node->parent_;
      node->parent_ = next_node;
      std::swap(node->left_, next_node->left_);
    } else {
      Node *node_left = node->left_;
      Node *node_right = node->right_;
      Node *next_node_parent = next_node->parent_;
      std::swap(node->left_, next_node->left_);
      std::swap(node->right_, next_node->right_);
      std::swap(node->parent_, next_node->parent_);
      std::swap(node->color_, next_node->color_);
      node_left->parent_ = next_node;
      node_right->parent_ = next_node;
      next_node_parent->left_ = node;
    }
  }
  if (node->color_ == ColorNode::BLACK) BalanceAfterDelete(node);

  if (node->IsNodeLeftChild())
    DeleteNodeAndAssignFakeLeft(node->parent_);
  else
    DeleteNodeAndAssignFakeRight(node->parent_);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::BalanceAfterDelete(Node *&node) {
  Node *parent = node->parent_;
  if (node != Root() && node->color_ == ColorNode::BLACK) {
    if (node->IsNodeLeftChild()) {
      Node *sibling = parent->right_;
      if (sibling->color_ == ColorNode::RED) {
        RotateLEFTSiblingRed(parent, sibling);
      }
      if ((!sibling->left_ || sibling->left_->color_ == ColorNode::BLACK) &&
          (!sibling->right_ || sibling->right_->color_ == ColorNode::BLACK)) {
        ChangeSiblingLeftBlackRightBlack(parent, sibling);
      } else {
        if ((!sibling->right_ || sibling->right_->color_ == ColorNode::BLACK)) {
          RotateLEFTSiblingRightBlack(sibling);
        }
        RotateLEFTSiblingRightRed(parent, sibling);
      }
    } else {
      Node *sibling = parent->left_;
      if (sibling->color_ == ColorNode::RED) {
        RotateRIGHTSiblingRed(parent, sibling);
      }
      if ((!sibling->left_ || sibling->left_->color_ == ColorNode::BLACK) &&
          (!sibling->right_ || sibling->right_->color_ == ColorNode::BLACK)) {
        ChangeSiblingLeftBlackRightBlack(parent, sibling);
      } else {
        if ((!sibling->left_ || sibling->left_->color_ == ColorNode::BLACK)) {
          RotateRIGHTSiblingLeftBlack(sibling);
        }
        RotateRIGHTSiblingLeftRed(parent, sibling);
      }
    }
  }
  node->color_ = ColorNode::BLACK;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateLEFTSiblingRed(Node *&parent,
                                                      Node *&sibling) {
  parent->color_ = ColorNode::RED;
  sibling->color_ = ColorNode::BLACK;
  sibling = sibling->left_;
  LeftRotate(parent);
}
template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateRIGHTSiblingRed(Node *&parent,
                                                       Node *&sibling) {
  parent->color_ = ColorNode::RED;
  sibling->color_ = ColorNode::BLACK;
  sibling = sibling->right_;
  RightRotate(parent);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::ChangeSiblingLeftBlackRightBlack(
    Node *&parent, Node *&sibling) {
  sibling->color_ = ColorNode::RED;
  if (parent->color_ == ColorNode::RED) {
    parent->color_ = ColorNode::BLACK;
  } else {
    BalanceAfterDelete(parent);
  }
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateLEFTSiblingRightBlack(Node *&sibling) {
  sibling->color_ = ColorNode::RED;
  sibling->left_->color_ = ColorNode::BLACK;
  RightRotate(sibling);
  sibling = sibling->parent_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateRIGHTSiblingLeftBlack(Node *&sibling) {
  sibling->color_ = ColorNode::RED;
  sibling->right_->color_ = ColorNode::BLACK;
  LeftRotate(sibling);
  sibling = sibling->parent_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateLEFTSiblingRightRed(Node *&parent,
                                                           Node *&sibling) {
  sibling->color_ = parent->color_;
  sibling->right_->color_ = ColorNode::BLACK;
  parent->color_ = ColorNode::BLACK;
  LeftRotate(parent);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RotateRIGHTSiblingLeftRed(Node *&parent,
                                                           Node *&sibling) {
  sibling->color_ = parent->color_;
  sibling->left_->color_ = ColorNode::BLACK;
  parent->color_ = ColorNode::BLACK;
  RightRotate(parent);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::DeleteNodeAndAssignFakeLeft(Node *&node) {
  Node *child = node->left_;
  if (child->left_ == fake_) {
    fake_->left_ = node;
    node->left_ = fake_;
  } else {
    node->left_ = nullptr;
  }
  delete child;
  --size_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::DeleteNodeAndAssignFakeRight(Node *&node) {
  Node *child = node->right_;
  if (child->right_ == fake_) {
    fake_->right_ = node;
    node->right_ = fake_;
  } else {
    node->right_ = nullptr;
  }
  delete child;
  --size_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::SwapNodes(Node *&node1, Node *&node2) {
  if (node1->left_ == fake_ || node1->right_ == fake_) {
    if (node1->left_ == fake_) {
      node2->left_ = fake_;
      fake_->left_ = node2;
    } else {
      node2->right_ = fake_;
      fake_->right_ = node2;
    }
  }
  if (node1 == Root())
    fake_->parent_ = node2;
  else if (node1->IsNodeLeftChild())
    node1->parent_->left_ = node2;
  else
    node1->parent_->right_ = node2;
  node2->parent_ = node1->parent_;
  delete node1;
  --size_;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::LeftRotate(Node *&x) {
  Node *y = x->right_;
  x->right_ = y->left_;
  if (y->left_) {
    y->left_->parent_ = x;
  }
  if (!x->parent_) {
    Root() = y;
  } else {
    if (x->IsNodeLeftChild()) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }
  }
  y->left_ = x;
  Node *x_parent = x->parent_;
  x->parent_ = y;
  y->parent_ = x_parent;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::RightRotate(Node *&x) {
  Node *y = x->left_;
  x->left_ = y->right_;
  if (y->right_) {
    y->right_->parent_ = x;
  }
  if (!x->parent_) {
    Root() = y;
  } else {
    if (x->IsNodeLeftChild()) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }
  }
  y->right_ = x;
  Node *x_parent = x->parent_;
  x->parent_ = y;
  y->parent_ = x_parent;
}

template <typename key_type, typename mapped_type>
std::pair<typename map<key_type, mapped_type>::iterator, bool>
map<key_type, mapped_type>::UniversalInsertOrAssign(const_reference value,
                                                    bool is_assign) {
  if (size_) {
    DirectionChild child = DirectionChild::LEFT_CHILD;
    std::pair<bool, Node *> res_pair = FindPosToInsert(Root(), value, child);
    bool res = res_pair.first;
    Node *node = res_pair.second;
    if (res) {
      InsertToNotBlank(node, child, value);
      BalancingTree(node);
    } else if (is_assign) {
      node->GetValue() = value.second;
    }
    return {iterator(node, *this), res};
  } else {
    Node *new_node = new Node(value);
    InsertToBlank(new_node);
    return {iterator(new_node, *this), true};
  }
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::Node::IsNodeLeftChild() const {
  if (!parent_) throw std::runtime_error("Parent is NULL");
  return this == parent_->left_;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::Node::IsNodeRightChild() const {
  return !IsNodeLeftChild();
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::Node::IsParentLeftChild() const {
  Node *grand_parent = parent_->parent_;
  if (!grand_parent) throw std::runtime_error("GrandParent is NULL");
  return parent_ == grand_parent->left_;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::Node::IsParentRightChild() const {
  return !IsParentLeftChild();
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::HasNodeLeftChild(Node *&node) const {
  return node->left_ && node->left_ != fake_;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::HasNodeRightChild(Node *&node) const {
  return node->right_ && node->right_ != fake_;
}

template <typename key_type, typename mapped_type>
int map<key_type, mapped_type>::HowMuchChildren(Node *&node) const {
  return (HasNodeLeftChild(node) << 1) + HasNodeRightChild(node);
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::Node *&map<key_type, mapped_type>::Root() {
  return fake_->parent_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::Node *&
map<key_type, mapped_type>::Node::Sibling() {
  if (IsNodeLeftChild()) return parent_->right_;
  return parent_->left_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::Node *&
map<key_type, mapped_type>::Node::Uncle() {
  Node *grand_parent = GrandParent();
  if (IsParentLeftChild()) return grand_parent->right_;
  return grand_parent->left_;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::Node *&
map<key_type, mapped_type>::Node::GrandParent() {
  if (!parent_) throw std::runtime_error("Parent is NULL");
  return parent_->parent_;
}

template <typename key_type, typename mapped_type>
const typename map<key_type, mapped_type>::key_type &
map<key_type, mapped_type>::Node::GetKey() const {
  return data_.first;
}

template <typename key_type, typename mapped_type>
typename map<key_type, mapped_type>::mapped_type &
map<key_type, mapped_type>::Node::GetValue() {
  return data_.second;
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::swap(map &other) {
  std::swap(fake_, other.fake_);
  std::swap(size_, other.size_);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::merge(map &other) {
  iterator other_it = other.begin();
  iterator other_it_end = other.end();
  while (other_it != other_it_end) {
    auto res = insert(*other_it);
    if (res.second) {
      other.erase(other_it++);
    } else {
      ++other_it;
    }
  }
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::contains(const key_type &key) const {
  return ContainsKey(fake_->parent_, key);
}

template <typename key_type, typename mapped_type>
template <class... Args>
std::vector<std::pair<typename map<key_type, mapped_type>::iterator, bool>>
map<key_type, mapped_type>::emplace(Args &&...args) {
  std::vector<std::pair<iterator, bool>> result;
  std::vector<value_type> arguments = {args...};
  for (const auto &val : arguments) {
    result.push_back(insert(val));
  }
  return result;
}

template <typename key_type, typename mapped_type>
bool map<key_type, mapped_type>::ContainsKey(Node *node,
                                             const key_type &key) const {
  if (!node || node == fake_) return false;
  key_type this_key = node->GetKey();
  if (key == this_key)
    return true;
  else if (key > this_key)
    return ContainsKey(node->right_, key);
  else
    return ContainsKey(node->left_, key);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::PrintTreeToDot(const std::string &file_name) {
  if (Root()) {
    std::ofstream stream(file_name);
    if (stream.is_open()) {
      if (size_) {
        stream << "digraph {" << std::endl;
        if (size_ == 1) {
          stream << "    " << Root()->GetKey() << ";" << std::endl;
        } else {
          std::vector<key_type> red_nodes;
          red_nodes.reserve(size_);
          Node *root = Root();
          PrintTree(root, stream, red_nodes);
          PrintRedNodes(stream, red_nodes);
        }
        stream << "}";
      }
      stream.close();
    }
  } else {
    std::ofstream stream(file_name);
    if (stream.is_open()) {
      stream << "";
      stream.close();
    }
  }
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::PrintTree(Node *node, std::ofstream &stream,
                                           std::vector<key_type> &red_nodes) {
  PrintLinks(node->left_, stream, red_nodes);
  PrintLinks(node->right_, stream, red_nodes);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::PrintLinks(Node *node, std::ofstream &stream,
                                            std::vector<key_type> &red_nodes) {
  if (!node || node == fake_) return;
  if (node->color_ == ColorNode::RED) red_nodes.push_back(node->data_.first);
  stream << "    " << node->parent_->data_.first << " -> " << node->data_.first
         << ";" << std::endl;
  PrintTree(node, stream, red_nodes);
}

template <typename key_type, typename mapped_type>
void map<key_type, mapped_type>::PrintRedNodes(
    std::ofstream &stream, const std::vector<key_type> &red_nodes) {
  for (const auto &val : red_nodes) {
    stream << "    " << val << " [color = red;]"
           << ";" << std::endl;
  }
}

}  // namespace s21

#endif  // SRC_S21_MAP_CC_