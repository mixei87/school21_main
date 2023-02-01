#ifndef SRC_S21_ARRAY_HH_
#define SRC_S21_ARRAY_HH_

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {

template <typename T, size_t size_>
class array {
 private:
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef size_t size_type;

  value_type head_[size_ ? size_ : 1];

 public:
  array();
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a);
  ~array();
  void operator=(array &&a);

  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  value_type *data();

  iterator begin();
  iterator end();

  bool empty();
  size_type size();
  size_type max_size();

  void swap(array &other);
  void fill(const_reference value);
};
}  // namespace s21
#include "s21_array.cc"
#endif  // SRC_S21_ARRAY_HH_
