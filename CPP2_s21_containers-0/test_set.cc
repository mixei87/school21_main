#include <gtest/gtest.h>

#include <set>

#include "s21_containers.h"

TEST(Set, Constructor_1) {
  s21::set<int> st;
  EXPECT_EQ(st.size(), 0);
  EXPECT_FALSE(st.empty());
}

TEST(Set, Constructor_2) {
  s21::set<int> st{3, 5, 8, 7, 2};
  EXPECT_EQ(st.size(), 5);
  EXPECT_TRUE(st.empty());
}

TEST(Set, Copy_Constructor) {
  s21::set<int> st{3, 5, 8, 7, 2};
  s21::set<int> result(st);
  s21::set<int>::iterator my_it = result.begin();
  EXPECT_EQ(result.size(), 5);
  EXPECT_EQ(*my_it, 2);
  ++my_it;
  EXPECT_EQ(*my_it, 3);
  EXPECT_TRUE(result.empty());
}

TEST(Set, Move_Constructor) {
  s21::set<int> st{3, 5, 8, 7, 2};
  s21::set<int> result = std::move(st);
  s21::set<int>::iterator my_it = result.begin();
  EXPECT_EQ(result.size(), 5);
  EXPECT_EQ(*my_it, 2);
  ++my_it;
  EXPECT_EQ(*my_it, 3);
  EXPECT_TRUE(result.empty());
}

TEST(Set, Clear) {
  s21::set<int> st{3, 5, 8, 7, 2};
  EXPECT_EQ(st.size(), 5);
  EXPECT_TRUE(st.empty());
  st.clear();
  EXPECT_EQ(st.size(), 0);
  EXPECT_FALSE(st.empty());
}

TEST(Set, Insert) {
  s21::set<int> st;
  st.insert(8);
  EXPECT_EQ(st.size(), 1);
  EXPECT_TRUE(st.empty());
  EXPECT_TRUE(st.insert(35).second);
  EXPECT_FALSE(st.insert(35).second);
  s21::set<int>::iterator iter;
  iter = st.insert(588).first;
  EXPECT_EQ(*iter, 588);
}

TEST(Set, Contains) {
  s21::set<int> st{8, 25, 35, 45, 11, 9, 7, 4, 14};
  EXPECT_TRUE(st.contains(45));
  EXPECT_TRUE(st.contains(11));
  EXPECT_TRUE(st.contains(7));
  EXPECT_TRUE(st.contains(9));
  EXPECT_FALSE(st.contains(4555));
  EXPECT_FALSE(st.contains(-5));
  EXPECT_FALSE(st.contains(987));
  EXPECT_FALSE(st.contains(654));
}

TEST(Set, Iterator_1) {
  s21::set<int> st{8, 25, 35, 45, 11, 9, 7, 4, 14};
  s21::set<int>::iterator my_it = st.begin();
  EXPECT_EQ(*my_it, 4);
  ++my_it;
  EXPECT_EQ(*my_it, 7);
  ++my_it;
  EXPECT_EQ(*my_it, 8);
  ++my_it;
  EXPECT_EQ(*my_it, 9);
}

TEST(Set, Iterator_2) {
  s21::set<int> st{8, 25, 35, 45, 11, 9, 7, 4, 14};
  s21::set<int>::iterator my_it = st.end();
  --my_it;
  EXPECT_EQ(*my_it, 45);
  --my_it;
  EXPECT_EQ(*my_it, 35);
  --my_it;
  EXPECT_EQ(*my_it, 25);
  --my_it;
  EXPECT_EQ(*my_it, 14);
  --my_it;
  EXPECT_EQ(*my_it, 11);
  --my_it;
  EXPECT_EQ(*my_it, 9);
  --my_it;
  EXPECT_EQ(*my_it, 8);
  --my_it;
  EXPECT_EQ(*my_it, 7);
}

TEST(Set, Find) {
  s21::set<int> st{8, 25, 35, 45, 11, 9, 7, 4, 14};
  s21::set<int>::iterator my_it = st.begin();
  EXPECT_EQ(st.find(4), my_it);
  ++my_it;
  EXPECT_EQ(st.find(7), my_it);
  --my_it;
  EXPECT_EQ(st.find(4), my_it);
  s21::set<int>::iterator iterator_25 = st.find(25);
  EXPECT_EQ(*iterator_25, 25);
}

TEST(Set, Merge) {
  s21::set<int> st{8, 25};
  s21::set<int> add{0, 15, 57, 25};
  st.merge(add);
  s21::set<int>::iterator my_it = st.begin();
  EXPECT_EQ(*my_it, 0);
  EXPECT_EQ(st.find(0), my_it);
  EXPECT_EQ(st.size(), 5);
}

TEST(Set, Erase) {
  s21::set<int> st{4, 6, 7, 3, 1};
  s21::set<int>::iterator iter = st.begin();
  ++iter;  // 3
  st.erase(iter);
  EXPECT_FALSE(st.contains(3));
  EXPECT_TRUE(st.contains(1));
  EXPECT_TRUE(st.contains(4));
  EXPECT_EQ(st.size(), 4);
}

TEST(Set, Erase_1) {
  s21::set<int> st{4, 8, 3, 1};
  s21::set<int>::iterator iter = st.begin();
  ++iter;
  st.erase(iter);
  EXPECT_FALSE(st.contains(3));
}

TEST(Set, Emplace) {
  s21::set<int> st{4, 7, 9};
  st.emplace(1);
  EXPECT_EQ(st.size(), 4);
  EXPECT_TRUE(st.empty());
  s21::set<int>::iterator iter;
  iter = st.begin();
  EXPECT_EQ(*iter, 1);
}

TEST(Set, Emplace_2) {
  s21::set<int> st{4, 7, 9};
  st.emplace(1, 2, 3);
  EXPECT_EQ(st.size(), 6);
  EXPECT_TRUE(st.empty());
  s21::set<int>::iterator iter;
  iter = st.begin();
  EXPECT_EQ(*iter, 1);
  ++iter;
  EXPECT_EQ(*iter, 2);
  ++iter;
  EXPECT_EQ(*iter, 3);
  ++iter;
}
