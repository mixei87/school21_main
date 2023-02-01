#ifndef SRC_S21_SET_H_
#define SRC_S21_SET_H_
#include <initializer_list>
#include <iostream>
#include <set>
#include <vector>

namespace s21 {
template <class Key>
class set {
 public:
  typedef Key key_type;
  typedef key_type value_type;
  typedef key_type& reference;
  typedef const key_type& const_reference;
  typedef size_t size_type;

  set();
  set(std::initializer_list<key_type> const& items);
  set(const set& s);
  set(set&& s);
  ~set();
  void operator=(set&& s);

  class Node {
   public:
    key_type data_;
    Node* pLeft_;
    Node* pRight_;
    Node* parent_;
    Node(key_type data_ = key_type(), Node* pRight_ = nullptr,
         Node* pLeft_ = nullptr, Node* parent_ = nullptr) {
      this->data_ = data_;
      this->pRight_ = pRight_;
      this->pLeft_ = pLeft_;
      this->parent_ = parent_;
    }
  };
  typedef class SetIterator {
   public:
    Node* iter_;

    SetIterator();
    SetIterator(Node& iter);
    SetIterator(const SetIterator& iter);
    ~SetIterator();

    SetIterator& operator=(const SetIterator& iter);
    bool operator==(const SetIterator& iter) const;
    bool operator!=(const SetIterator& iter) const;
    reference operator*();
    void operator++();
    void operator--();
  } iterator;
  iterator begin();
  iterator end();

  typedef const SetIterator const_iterator;
  const_iterator cbegin() const;
  const_iterator cend() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;
  void clear();

  /*вставляет Элемент и возвращает итератор туда, где элемент находится в
  контейнере, и логическое значение, обозначающее, имела ли место вставка*/
  std::pair<iterator, bool> insert(const value_type& value);

  /*проверяет, содержит ли контейнер элемент с определенным ключом*/
  bool contains(const_reference key);

  /*Находит элемент с определенным ключом
  Возвращает итератор элемента*/
  iterator find(const_reference key);

  /*стирает элемент в позиции*/
  void erase(iterator pos);

  /*меняет содержимое*/
  void swap(set& other);

  /*соединяет узлы из другого контейнера*/
  void merge(set& other);

  /*Удаляем все элементы*/
  void deleteAllNode(Node*& dNode);

  template <class... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args&&... args);

  /*Удаляем один элемент*/
  void deleteOneNode(Node* dNode, const key_type& key);
  void createFake(Node* node);
  void cleanFake();
  iterator addNode(Node* addNode, const key_type& key);
  void PrintSet();

 protected:
  Node* rootTree_;
  Node* fake_;
  int size_;
};
}  // namespace s21
#include "s21_set.cc"
#endif  // SRC_S21_SET_CC_
