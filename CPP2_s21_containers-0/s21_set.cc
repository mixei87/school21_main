#ifndef SRC_S21_SET_CC_
#define SRC_S21_SET_CC_
#include "s21_set.h"

#include <iostream>

namespace s21 {

template <class key_type>
set<key_type>::set() : rootTree_(nullptr), size_(0) {
  createFake(rootTree_);
}

template <class key_type>
set<key_type>::set(std::initializer_list<key_type> const& items) : set() {
  for (auto i = items.begin(); i != items.end(); i++) {
    insert(*i);
  }
}

template <class key_type>
set<key_type>::set(const set& s) : set() {
  rootTree_ = s.rootTree_;
  iterator iter;
  iter.iter_ = s.rootTree_;
  size_ = s.size_;
  while (iter.iter_->pLeft_ != nullptr) {
    iter.iter_ = iter.iter_->pLeft_;
  }
  for (int i = 0; i < s.size_; i++) {
    this->insert(*iter);
    ++iter;
  }
}

template <class key_type>
set<key_type>::set(set&& s) : rootTree_(nullptr), size_(0) {
  if (rootTree_ != s.rootTree_) {
    std::swap(this->rootTree_, s.rootTree_);
    std::swap(this->size_, s.size_);
  }
}

template <class key_type>
void set<key_type>::operator=(set&& s) {
  if (rootTree_ != s.rootTree_) {
    clear();
    std::swap(this->rootTree_, s.rootTree_);
    std::swap(this->size_, s.size_);
    std::swap(this->fake_, s.fake_);
  }
}

template <class key_type>
set<key_type>::~set() {
  deleteAllNode(rootTree_);
  fake_ = nullptr;
  delete rootTree_;
  rootTree_ = nullptr;
}

template <class key_type>
void set<key_type>::createFake(Node* node) {
  fake_ = new Node;
  fake_->data_ = size_;
  fake_->parent_ = node;
}

template <class key_type>
void set<key_type>::cleanFake() {
  fake_->data_ = 0;
  fake_->parent_ = nullptr;
  fake_ = nullptr;
}

template <class key_type>
set<key_type>::SetIterator::SetIterator() : iter_{nullptr} {}

template <class key_type>
set<key_type>::SetIterator::SetIterator(Node& iter) : iter_{&iter} {}

template <class key_type>
set<key_type>::SetIterator::SetIterator(const SetIterator& iter)
    : iter_{iter.iter_} {}

template <class key_type>
set<key_type>::iterator::~SetIterator() {
  this->iter_ = nullptr;
}

template <class key_type>
typename set<key_type>::iterator set<key_type>::begin() {
  iterator first_element(*this->rootTree_);
  if (first_element.iter_ != nullptr) {
    while (first_element.iter_->pLeft_ != nullptr) {
      first_element.iter_ = first_element.iter_->pLeft_;
    }
  }
  return first_element;
}

template <class key_type>
typename set<key_type>::iterator set<key_type>::end() {
  iterator after_last_leaf(*this->fake_);
  return after_last_leaf;
}

template <class key_type>
typename set<key_type>::const_iterator set<key_type>::cbegin() const {
  const_iterator first_element(*this->begin());
  return first_element;
}

template <class key_type>
typename set<key_type>::const_iterator set<key_type>::cend() const {
  const_iterator last_element(*this->end());
  return last_element;
}

template <class key_type>
typename set<key_type>::SetIterator& set<key_type>::SetIterator::operator=(
    const SetIterator& iter) {
  iter_ = iter.iter_;
  return *this;
}

template <class key_type>
bool set<key_type>::iterator::operator==(const SetIterator& iter) const {
  if (iter_ == iter.iter_) {
    return true;
  }
  return false;
}

template <class key_type>
bool set<key_type>::iterator::operator!=(const SetIterator& iter) const {
  if (iter_ != iter.iter_) {
    return true;
  }
  return false;
}

template <class key_type>
typename set<key_type>::reference set<key_type>::SetIterator::operator*() {
  if (iter_ == nullptr) {
    throw std::invalid_argument("iter_ = null pointer (operator*)");
  }
  return iter_->data_;
}

template <class key_type>
void set<key_type>::SetIterator::operator++() {
  if (iter_ == nullptr) {
    throw std::invalid_argument("iter_ = null pointer (operator++)");
  }
  if (iter_->pRight_ != nullptr) {
    iter_ = iter_->pRight_;
    while (iter_->pLeft_ != nullptr) {
      iter_ = iter_->pLeft_;
    }
  } else {
    auto temp = iter_->data_;
    iterator tempI;
    tempI.iter_ = iter_;
    if (iter_->parent_ != nullptr) {
      iter_ = iter_->parent_;
      if (iter_->data_ < temp) {
        while (iter_->data_ < temp) {
          if (iter_->parent_ == nullptr) {
            iter_ = tempI.iter_;
            break;
          } else {
            iter_ = iter_->parent_;
          }
        }
      }
    }
  }
}

template <class key_type>
void set<key_type>::SetIterator::operator--() {
  if (iter_ == nullptr) {
    throw std::invalid_argument("iter_ = null pointer (operator--)");
  }
  if (iter_->pLeft_ != nullptr) {
    iter_ = iter_->pLeft_;
    while (iter_->pRight_ != nullptr) {
      iter_ = iter_->pRight_;
    }
  } else {
    Node* leftChild = nullptr;
    do {
      leftChild = iter_;
      iter_ = iter_->parent_;
    } while (iter_->parent_ && iter_->pLeft_ == leftChild);
  }
}

template <class key_type>
std::pair<typename set<key_type>::iterator, bool> set<key_type>::insert(
    const_reference value) {
  std::pair<iterator, bool> resultPair;
  if (rootTree_ == nullptr) {
    rootTree_ = new Node(value);
    ++size_;
    rootTree_->pRight_ = fake_;
    resultPair.first.iter_ = rootTree_;
    fake_->parent_ = rootTree_;
    resultPair.second = true;
  } else {
    if (rootTree_->data_ != value) {
      int sizeFlag = size_;
      resultPair.first.iter_ = addNode(rootTree_, value).iter_;
      if (sizeFlag != size_) {
        resultPair.second = true;
      }
    }
  }
  return resultPair;
}

template <class key_type>
void set<key_type>::clear() {
  if (rootTree_->pLeft_ != nullptr) {
    deleteAllNode(rootTree_->pLeft_);
  }
  if (rootTree_->pRight_ != nullptr) {
    deleteAllNode(rootTree_->pRight_);
  }
  rootTree_->data_ = 0;
  size_ = 0;
}

template <class key_type>
bool set<key_type>::contains(const_reference key) {
  bool result = false;
  if (this->rootTree_ != nullptr) {
    iterator iter;
    iter = this->begin();
    for (int i = 0; i < this->size_; i++) {
      if (*iter == key) {
        result = true;
      }
      ++iter;
    }
  }
  return result;
}

template <class key_type>
bool set<key_type>::empty() const {
  return bool(size_);
}

template <class key_type>
typename set<key_type>::size_type set<key_type>::size() const {
  return size_;
}

template <class key_type>
typename set<key_type>::size_type set<key_type>::max_size() const {
  return (SIZE_MAX / (sizeof(rootTree_) * 3 + sizeof(this->rootTree_->data_)));
}

template <class key_type>
typename set<key_type>::iterator set<key_type>::find(const_reference key) {
  iterator iter;
  if (contains(key)) {
    iter = begin();
    while (*iter != key) {
      ++iter;
    }
  } else {
    iter = end();
  }
  return iter;
}

template <class key_type>
void set<key_type>::erase(iterator deleteIter) {
  if (deleteIter.iter_ != nullptr) {
    deleteOneNode(this->rootTree_, deleteIter.iter_->data_);
  }
}

template <class key_type>
void set<key_type>::swap(set& other) {
  if (rootTree_ != other.rootTree_) {
    std::swap(rootTree_, other.rootTree_);
  }
}

template <class key_type>
void set<key_type>::merge(set& other) {
  if (rootTree_ != other.rootTree_) {
    iterator iter = other.begin();
    for (int i = 0; i < other.size_; i++) {
      insert(*iter);
      ++iter;
    }
    other.clear();
  }
}

template <class key_type>
void set<key_type>::deleteAllNode(Node*& deleteNode) {
  if (deleteNode != nullptr) {
    if (deleteNode->pLeft_ != nullptr) {
      deleteAllNode(deleteNode->pLeft_);
    }
    if (deleteNode->pRight_ != nullptr) {
      deleteAllNode(deleteNode->pRight_);
    }
    deleteNode = nullptr;
    size_ = 0;
  }
}

template <class key_type>
void set<key_type>::deleteOneNode(Node* deleteNode, const key_type& key) {
  if (deleteNode != nullptr) {
    if (deleteNode->data_ == key) {
      if (deleteNode->pRight_ == nullptr && deleteNode->pLeft_ == nullptr) {
        deleteNode = nullptr;
        --size_;
      } else if (deleteNode->pLeft_ != nullptr) {
        deleteNode->pLeft_->parent_ = deleteNode->parent_;
        Node* tempNode = deleteNode->pRight_;
        deleteNode = deleteNode->pLeft_;
        if (tempNode != nullptr) {
          deleteNode->pRight_ = tempNode;
        }
        --size_;
      } else if (deleteNode->pLeft_ == nullptr &&
                 deleteNode->pRight_ != nullptr) {
        Node* tempNode = deleteNode->pRight_;
        deleteNode->parent_->pRight_ = tempNode;
        tempNode->parent_ = deleteNode->parent_;
        tempNode->pLeft_ = deleteNode->pLeft_;
        tempNode->pRight_ = deleteNode->pRight_;
        deleteNode = tempNode;
        --size_;
      }
    } else if (key < deleteNode->data_) {
      deleteOneNode(deleteNode->pLeft_, key);
    } else if (key > deleteNode->data_) {
      deleteOneNode(deleteNode->pRight_, key);
    }
  }
}

template <class key_type>
void set<key_type>::PrintSet() {
  iterator iter = begin();
  if (iter.iter_ == nullptr) {
    std::cout << "No elements" << std::endl;
  } else {
    for (int i = 0; i < size_; i++) {
      std::cout << *iter << std::endl;
      ++iter;
    }
  }
}

template <class key_type>
typename set<key_type>::iterator set<key_type>::addNode(Node* treeNode,
                                                        const key_type& value) {
  iterator result;
  if (treeNode->data_ > value) {
    if (treeNode->pLeft_ != nullptr) {
      result = addNode(treeNode->pLeft_, value);
    } else {
      treeNode->pLeft_ = new Node(value, nullptr, nullptr, treeNode);
      ++size_;
      result.iter_ = treeNode->pLeft_;
    }
  } else if (treeNode->data_ < value) {
    if (treeNode->pRight_ != nullptr && treeNode->pRight_ != fake_) {
      result = addNode(treeNode->pRight_, value);
    } else {
      if (treeNode->pRight_ == nullptr) {
        treeNode->pRight_ = new Node(value, nullptr, nullptr, treeNode);
        ++size_;
        result.iter_ = treeNode->pRight_;
      } else if (treeNode->pRight_ == fake_) {
        cleanFake();
        treeNode->pRight_ = new Node(value, nullptr, nullptr, treeNode);
        ++size_;
        result.iter_ = treeNode->pRight_;
        createFake(treeNode->pRight_);
        treeNode->pRight_->pRight_ = fake_;
      }
    }
  }
  return result;
}

template <class key_type>
template <class... Args>
std::vector<std::pair<typename set<key_type>::iterator, bool>>
set<key_type>::emplace(Args&&... args) {
  std::vector<std::pair<typename set<key_type>::iterator, bool>> result;
  std::vector<key_type> arguments = {args...};
  for (auto& a : arguments) {
    result.push_back(insert(a));
  }
  return result;
}

}  // namespace s21

// int main() {
//     s21::set<int> set1{8, 5, 1};
//     set1.emplace(99, 11, 57, 48);
//     set1.PrintSet();

//     return 0;
// }
#endif  // SRC_S21_SET_CC_
