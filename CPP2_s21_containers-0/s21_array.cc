#ifndef SRC_S21_ARRAY_CC_
#define SRC_S21_ARRAY_CC_
#include "s21_array.h"

namespace s21 {
/*Конструкторы, деструкторы ************************************/

template <class value_type, size_t size_>
array<value_type, size_>::array() {
  for (size_type i = 0; i < size_; ++i) *(head_ + i) = 0;
}

template <class value_type, size_t size_>
array<value_type, size_>::array(
    std::initializer_list<value_type> const &items) {
  size_type i = 0;
  for (const auto &val : items) {
    if (i >= size_) break;
    *(head_ + i) = val;
    ++i;
  }
  for (; i < size_; ++i) *(head_ + i) = 0;
}

/* Copy Constructor*/
template <class value_type, size_t size_>
array<value_type, size_>::array(const array &a) {
  if (this != &a) {
    for (size_type i = 0; i < size_; ++i) *(head_ + i) = *(a.head_ + i);
  }
}

/* Move Constructor*/
template <class value_type, size_t size_>
array<value_type, size_>::array(array &&a) {
  if (this != &a) {
    std::move(std::begin(a.head_), std::end(a.head_), &head_[0]);
  }
}

template <class value_type, size_t size_>
array<value_type, size_>::~array() {}

template <class value_type, size_t size_>
void array<value_type, size_>::operator=(array &&a) {
  array(std::move(a));
}
/***************************************************************/

template <class value_type, size_t size_>
typename array<value_type, size_>::reference array<value_type, size_>::at(
    size_type pos) {
  if (pos < size_) return operator[](pos);
  throw std::out_of_range("Out of range");
}

template <class value_type, size_t size_>
typename array<value_type, size_>::reference
array<value_type, size_>::operator[](size_type pos) {
  return *(head_ + pos);
}

template <class value_type, size_t size_>
typename array<value_type, size_>::const_reference
array<value_type, size_>::front() {
  return *head_;
}

template <class value_type, size_t size_>
typename array<value_type, size_>::const_reference
array<value_type, size_>::back() {
  return *(head_ + size_ - 1);
}

template <class value_type, size_t size_>
typename array<value_type, size_>::value_type *
array<value_type, size_>::data() {
  return head_;
}

template <class value_type, size_t size_>
typename array<value_type, size_>::iterator array<value_type, size_>::begin() {
  return head_;
}

template <class value_type, size_t size_>
typename array<value_type, size_>::iterator array<value_type, size_>::end() {
  return head_ + size_;
}

template <class value_type, size_t size_>
bool array<value_type, size_>::empty() {
  return !size_;
}

template <class value_type, size_t size_>
typename array<value_type, size_>::size_type array<value_type, size_>::size() {
  return size_;
}

template <class value_type, size_t size_>
typename array<value_type, size_>::size_type
array<value_type, size_>::max_size() {
  return size_;
}

template <class value_type, size_t size_>
void array<value_type, size_>::swap(array &other) {
  if (this != &other && size_ == other.size())
    for (size_type i = 0; i < size_; ++i) std::swap(head_[i], other.head_[i]);
}

template <class value_type, size_t size_>
void array<value_type, size_>::fill(const_reference value) {
  for (size_type i = 0; i < size_; ++i) *(head_ + i) = value;
}

}  // namespace s21

#endif  // SRC_S21_ARRAY_CC_
