#ifndef SRC_S21_MAP_H_
#define SRC_S21_MAP_H_

#include <unistd.h>

#include <cstddef>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace s21 {

template <typename Key, typename T>
class map {
 private:
  typedef Key key_type;
  typedef T mapped_type;
  typedef std::pair<const Key, T> value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef size_t size_type;
  enum ColorNode { RED, BLACK };
  enum DirectionChild { LEFT_CHILD, RIGHT_CHILD };

  class Node {
   public:
    Node(value_type data, Node *parent = nullptr);
    Node(value_type data, ColorNode color);
    value_type data_;
    Node *parent_;
    Node *left_;
    Node *right_;
    ColorNode color_;

    const key_type &GetKey() const;
    mapped_type &GetValue();
    Node *&Sibling();
    Node *&GrandParent();
    Node *&Uncle();
    bool IsNodeLeftChild() const;
    bool IsNodeRightChild() const;
    bool IsParentLeftChild() const;
    bool IsParentRightChild() const;
  };

  int size_;
  Node *fake_;

  void CreateFake();
  Node *&Root();
  std::pair<bool, Node *> FindPosToInsert(
      Node *node, const_reference value,
      DirectionChild &child = DirectionChild::LEFT_CHILD);
  void InsertToBlank(Node *&new_node);
  void InsertToNotBlank(Node *&node, const DirectionChild &child,
                        const_reference value);
  void BalancingTree(Node *&node);
  void ChangeColorParentAndGrandparent(Node *&node);
  void LeftRotate(Node *&node);
  void RightRotate(Node *&node);
  void PrintTree(Node *node, std::ofstream &stream,
                 std::vector<key_type> &all_nodes);
  void PrintLinks(Node *node, std::ofstream &stream,
                  std::vector<key_type> &all_nodes);
  void PrintRedNodes(std::ofstream &stream,
                     const std::vector<key_type> &all_nodes);
  bool HasNodeLeftChild(Node *&node) const;
  bool HasNodeRightChild(Node *&node) const;
  int HowMuchChildren(Node *&node) const;
  void SwapNodes(Node *&node1, Node *&node2);
  void BalanceAfterDelete(Node *&node);
  void DeleteNodeAndAssignFakeLeft(Node *&node);
  void DeleteNodeAndAssignFakeRight(Node *&node);
  void ChangeSiblingLeftBlackRightBlack(Node *&parent, Node *&sibling);

  void RotateLEFTSiblingRed(Node *&parent, Node *&sibling);
  void RotateLEFTSiblingRightBlack(Node *&sibling);
  void RotateLEFTSiblingRightRed(Node *&parent, Node *&sibling);

  void RotateRIGHTSiblingRed(Node *&parent, Node *&sibling);
  void RotateRIGHTSiblingLeftBlack(Node *&sibling);
  void RotateRIGHTSiblingLeftRed(Node *&parent, Node *&sibling);

  void DeleteAll(Node *node);
  Node *CopyAll(Node *&m_node, const Node *&m_fake, Node *node);
  bool ContainsKey(Node *node, const key_type &key) const;

 public:
  map();
  map(std::initializer_list<value_type> const &items);
  map(const map &m);
  map(map &&m);
  ~map();
  void operator=(map &&m);

  void PrintTreeToDot(const std::string &file_name);

  typedef class MapIterator {
   protected:
    const map *obj_;

   public:
    Node *iter_;
    MapIterator();
    MapIterator(Node *&iter, const map &obj);
    MapIterator(const MapIterator &it) = default;
    ~MapIterator() = default;
    MapIterator &operator=(const MapIterator &other);
    reference operator*();
    value_type *operator->();
    MapIterator &operator++();
    MapIterator operator++(int);
    MapIterator &operator--();
    bool operator==(const MapIterator &other) const;
    bool operator!=(const MapIterator &other) const;

  } iterator;

  iterator begin();
  iterator end();

  T &at(const key_type &key);
  T &operator[](const key_type &key);

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  std::pair<iterator, bool> insert(const_reference value);
  std::pair<iterator, bool> insert(const key_type &key, const mapped_type &obj);
  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &obj);
  void erase(iterator pos);
  void swap(map &other);
  void merge(map &other);

  bool contains(const key_type &key) const;

  template <class... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args);

 private:
  std::pair<iterator, bool> UniversalInsertOrAssign(const_reference value,
                                                    bool is_assign);
};
}  // namespace s21
#include "s21_map.cc"
#endif  // SRC_S21_MAP_H_
