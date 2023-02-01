#ifndef SRC_S21_MULTISET_CC_
#define SRC_S21_MULTISET_CC_
#include "s21_multiset.h"

namespace s21 {

template <class key_type>
multiset<key_type>::multiset() : set<key_type>::set() {}

template <class key_type>
multiset<key_type>::multiset(const multiset<key_type>& other)
    : set<key_type>::set(other) {}

template <class key_type>
multiset<key_type>::multiset(multiset<key_type>&& other)
    : set<key_type>::set(other) {}

template <class key_type>
multiset<key_type>::multiset(std::initializer_list<key_type> const& items) {
  for (auto& i : items) {
    insert(i);
  }
}

template <class key_type>
multiset<key_type>::~multiset() {}

template <class key_type>
typename multiset<key_type>::iterator multiset<key_type>::insert(
    const_reference key) {
  iterator iterResult;
  if (this->rootTree_ == nullptr) {
    this->rootTree_ = new NodeSet(key);
    this->rootTree_->pRight_ = this->fake_;
    this->fake_->parent_ = this->rootTree_;
    ++this->size_;
    iterResult.iter_ = this->rootTree_;
  } else {
    iterResult = addOneNode(this->rootTree_, key);
  }
  return iterResult;
}

template <class key_type>
typename multiset<key_type>::iterator multiset<key_type>::addOneNode(
    NodeSet* treeNode, const key_type& value) {
  iterator result;
  if (treeNode->data_ > value) {
    if (treeNode->pLeft_ != nullptr) {
      result = addOneNode(treeNode->pLeft_, value);
    } else {
      treeNode->pLeft_ = new NodeSet(value, nullptr, nullptr, treeNode);
      ++this->size_;
      result.iter_ = treeNode->pLeft_;
    }
  } else if (treeNode->data_ <= value) {
    if (treeNode->pRight_ != nullptr && treeNode->pRight_ != this->fake_) {
      result = addOneNode(treeNode->pRight_, value);
    } else {
      if (treeNode->pRight_ == nullptr) {
        treeNode->pRight_ = new NodeSet(value, nullptr, nullptr, treeNode);
        ++this->size_;
        result.iter_ = treeNode->pRight_;
      } else if (treeNode->pRight_ == this->fake_) {
        this->cleanFake();
        treeNode->pRight_ = new NodeSet(value, nullptr, nullptr, treeNode);
        ++this->size_;
        result.iter_ = treeNode->pRight_;
        this->createFake(treeNode->pRight_);
        treeNode->pRight_->pRight_ = this->fake_;
      }
    }
  }
  return result;
}

template <class key_type>
typename multiset<key_type>::iterator multiset<key_type>::upper_bound(
    const key_type& key) {
  iterator iterResult;
  iterResult = this->find(key);
  if (iterResult == this->end()) {
    if (key < *this->begin()) {
      iterResult = this->begin();
    } else {
      iterResult = this->begin();
      while (key >= *iterResult && iterResult.iter_ != nullptr) {
        ++iterResult;
      }
    }
  } else {
    auto temp = *iterResult;
    ++iterResult;
    auto result = *iterResult;
    while (temp == result) {
      ++iterResult;
      result = *iterResult;
    }
  }
  return iterResult;
}

template <class key_type>
typename multiset<key_type>::iterator multiset<key_type>::lower_bound(
    const key_type& key) {
  iterator iterResult;
  iterResult = this->find(key);
  if (iterResult == this->end()) {
    if (key < *this->begin()) {
      iterResult = this->begin();
    } else {
      iterResult = this->begin();
      while (key > *iterResult) {
        ++iterResult;
      }
    }
  }
  return iterResult;
}

template <class key_type>
void multiset<key_type>::erase(iterator deleteIter) {
  if (deleteIter.iter_ != nullptr) {
    delMSetOneNode(this->rootTree_, deleteIter.iter_->data_);
  }
}

template <class key_type>
void multiset<key_type>::delMSetOneNode(NodeSet* deleteNode,
                                        const key_type& key) {
  if (deleteNode != nullptr) {
    if (deleteNode->data_ == key) {
      if (deleteNode->pLeft_ == nullptr && deleteNode->pRight_ == nullptr) {
        if (deleteNode->parent_->pLeft_ == deleteNode) {
          deleteNode->parent_->pLeft_ = nullptr;
        } else if (deleteNode->parent_->pRight_ == deleteNode) {
          deleteNode->parent_->pRight_ = nullptr;
        }
        deleteNode = nullptr;
        --this->size_;
      } else if (deleteNode->pLeft_ != nullptr) {
        NodeSet* tempNode = deleteNode->pLeft_;
        while (tempNode->pRight_ != nullptr) {
          tempNode = tempNode->pRight_;
        }
        if (tempNode->parent_->pLeft_ == tempNode) {
          tempNode->parent_->pLeft_ = tempNode->pLeft_;
        } else if (tempNode->parent_->pRight_ == tempNode) {
          tempNode->parent_->pRight_ = tempNode->pLeft_;
        }
        if (tempNode->pLeft_ != nullptr) {
          tempNode->pLeft_->parent_ = tempNode->parent_;
        }
        tempNode->pRight_ = deleteNode->pRight_;
        tempNode->pLeft_ = deleteNode->pLeft_;
        tempNode->parent_ = deleteNode->parent_;
        if (deleteNode->parent_ != nullptr) {
          if (deleteNode->parent_->pLeft_ == deleteNode) {
            deleteNode->parent_->pLeft_ = tempNode;
          } else if (deleteNode->parent_->pRight_ == deleteNode) {
            deleteNode->parent_->pRight_ = tempNode;
          }
        }
        deleteNode = nullptr;
        --this->size_;
      } else if (deleteNode->pRight_ != nullptr) {
        if (deleteNode->parent_ != nullptr) {
          if (deleteNode->parent_->pLeft_ == deleteNode) {
            deleteNode->parent_->pLeft_ = deleteNode->pRight_;
          } else if (deleteNode->parent_->pRight_ == deleteNode) {
            deleteNode->parent_->pRight_ = deleteNode->pRight_;
          }
        }
        deleteNode->pRight_->parent_ = deleteNode->parent_;
        deleteNode = nullptr;
        --this->size_;
      }
    } else if (key < deleteNode->data_) {
      delMSetOneNode(deleteNode->pLeft_, key);
    } else if (key > deleteNode->data_) {
      delMSetOneNode(deleteNode->pRight_, key);
    }
  }
}

template <class key_type>
typename multiset<key_type>::size_type multiset<key_type>::count(
    const key_type& value) {
  size_type count = 0;
  if (this->contains(value)) {
    iterator it = this->find(value);
    while (*it == value) {
      ++count;
      ++it;
    }
  }
  return count;
}

template <class key_type>
std::pair<typename multiset<key_type>::iterator,
          typename multiset<key_type>::iterator>
multiset<key_type>::equal_range(const key_type& key) {
  return std::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
}

template <class key_type>
template <class... Args>
typename multiset<key_type>::iterator multiset<key_type>::emplace(
    Args&&... args) {
  iterator result;
  std::vector<key_type> arguments = {args...};
  for (auto& a : arguments) {
    result = insert(a);
  }
  return result;
}

}  // namespace s21

// int main() {
//     s21::multiset<int> mset {7, 6, 3, 4, 5};
//     mset.emplace(1, 2, 2, 9, 15, 15, 15);
//     mset.PrintSet();

//     // s21::multiset<int>::iterator iter = mset.begin();
//     // std::cout << sizeof(bool) << "\n" << std::endl;
//     // int* x = new int(1);
//     // std::cout << sizeof(x) << std::endl;
//     //std::cout << mset.max_size() << std::endl;
//     return 0;
// }

#endif  // SRC_S21_MULTISET_CC_