#ifndef SRC_S21_STACK_H_
#define SRC_S21_STACK_H_
#include <initializer_list>
#include <iostream>
#include <set>
#include <vector>

namespace s21 {
template <typename T>
class stack {
 public:
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  stack();
  ~stack();
  stack(std::initializer_list<value_type> const &s);
  stack(const stack &s);
  stack(stack &&s);
  reference operator=(stack &&s);

  const_reference top();

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void push(const_reference value);
  void pop();
  void swap(stack &other);

  void printStack();
  void swap_stack(stack &&other);
  template <class... Args>
  void emplace_back(Args &&...args);

 private:
  class Node {
   public:
    value_type data_;
    Node *pNext_;
    Node(value_type data_ = value_type(), Node *pNext_ = nullptr) {
      this->data_ = data_;
      this->pNext_ = pNext_;
    }
  };
  Node *head_;
  int size_;
};
}  // namespace s21
#include "s21_stack.cc"
#endif  // SRC_S21_STACK_H_
