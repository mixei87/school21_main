#ifndef SRC_S21_MULTISET_H_
#define SRC_S21_MULTISET_H_
#include <vector>

#include "s21_set.h"

namespace s21 {
template <class Key>
class multiset : public set<Key> {
 public:
  typedef Key key_type;
  typedef key_type value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef typename set<key_type>::iterator iterator;
  typedef typename set<key_type>::Node NodeSet;

  multiset();
  multiset(std::initializer_list<value_type> const& items);
  multiset(const multiset<key_type>& other);
  multiset(multiset<key_type>&& other);
  ~multiset();

  iterator insert(const value_type& value);

  /*стирает элемент в позиции*/
  void erase(iterator pos);

  size_type count(const Key& key);
  std::pair<iterator, iterator> equal_range(const Key& key);

  iterator lower_bound(const Key& key);
  iterator upper_bound(const Key& key);
  template <class... Args>
  iterator emplace(Args&&... args);

 private:
  multiset<key_type>& operator=(multiset&& other);
  iterator addOneNode(NodeSet* addNode, const key_type& key);
  void delMSetOneNode(NodeSet* dNode, const key_type& key);
};

}  // namespace s21
#include "s21_multiset.cc"
#endif  // SRC_S21_MULTISET_CC_
