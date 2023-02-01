#ifndef SRC_S21_QUEUE_H_
#define SRC_S21_QUEUE_H_
#include <initializer_list>
#include <iostream>
#include <set>
#include <vector>

namespace s21 {
template <typename T>
class queue {
 public:
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  queue();
  queue(std::initializer_list<value_type> const &items);
  queue(const queue &q);
  queue(queue &&q);
  ~queue();
  reference operator=(queue &&q);

  const_reference front();
  const_reference back();

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void push(const_reference value);
  void pop();
  void swap(queue &other);
  template <class... Args>
  void emplace_back(Args &&...args);

  /*For debug*/
  reference operator[](const int index);
  void swap_queue(queue &&other);
  void PrintQ();

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

  Node *first_;
  Node *last_;
  int size_;
};
}  // namespace s21
#include "s21_queue.cc"
#endif  // SRC_S21_QUEUE_H_