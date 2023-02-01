#include <gtest/gtest.h>

#include <set>

#include "s21_containersplus.h"

TEST(Multiset, Constructor) {
  s21::multiset<int> st;
  EXPECT_EQ(st.size(), 0);
  EXPECT_FALSE(st.empty());
}

TEST(Multiset, Constructor_1) {
  s21::multiset<int> st{1, 3, 5};
  EXPECT_EQ(st.size(), 3);
  EXPECT_TRUE(st.empty());
  s21::multiset<int>::iterator it = st.begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(3, *it);
  ++it;
  EXPECT_EQ(5, *it);
}

TEST(Multiset, Constructor_Copy) {
  s21::multiset<int> st{1, 3, 5};
  s21::multiset<int> st2(st);
  s21::multiset<int>::iterator it = st2.begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(3, *it);
  ++it;
  EXPECT_EQ(5, *it);
  EXPECT_EQ(st.size(), 3);
  EXPECT_TRUE(st.empty());
  EXPECT_EQ(st2.size(), 3);
  EXPECT_TRUE(st2.empty());
}

TEST(Multiset, Constructor_Move) {
  s21::multiset<int> st{1, 3, 5};
  s21::multiset<int> st2 = std::move(st);
  s21::multiset<int>::iterator it = st2.begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(3, *it);
  ++it;
  EXPECT_EQ(5, *it);
  EXPECT_EQ(st2.size(), 3);
  EXPECT_TRUE(st2.empty());
}

TEST(Multiset, Erase_1) {
  s21::multiset<int> st{7, 3, 1, 5};
  s21::multiset<int>::iterator it = st.begin();
  st.erase(it);
  s21::multiset<int>::iterator it2 = st.begin();
  EXPECT_EQ(3, *it2);
  ++it2;
  EXPECT_EQ(5, *it2);
  EXPECT_EQ(st.size(), 3);
  EXPECT_TRUE(st.empty());
}

TEST(Multiset, Erase_2) {
  s21::multiset<int> st{7, 3, 1, 5, 151, -15, 879, 45, 4, 0};
  s21::multiset<int>::iterator it = st.begin();
  ++it;
  ++it;
  ++it;
  ++it;
  st.erase(it);

  it = st.begin();
  ++it;
  ++it;
  ++it;
  ++it;
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 7);
  EXPECT_EQ(st.size(), 9);
  EXPECT_TRUE(st.empty());
}

TEST(Multiset, Erase_3) {
  s21::multiset<int> st{7, 3, 1, 5, 151, -15, 879, 45, 4, 0};
  s21::multiset<int>::iterator it = st.begin();
  s21::multiset<int>::iterator it2 = it;
  ++it2;
  for (; it2 != st.end(); ++it2) {
    st.erase(it);
    it = it2;
  }
  st.erase(it);
  EXPECT_EQ(st.size(), 0);
  EXPECT_FALSE(st.empty());
}

TEST(Multiset, Erase_4) {
  s21::multiset<int> st{7, 3, 1, 5, 151, -15, 879, 45, 4, 0};
  s21::multiset<int>::iterator it = st.end();
  --it;
  st.erase(it);
  EXPECT_EQ(st.size(), 9);
  EXPECT_TRUE(st.empty());
}

TEST(Multiset, Lower_bound) {
  s21::multiset<int> st{7, 1, 3, 3, 4, 4, 4, 3, 5};
  std::multiset<int> fset{7, 1, 3, 3, 4, 4, 4, 3, 5};
  EXPECT_EQ(*st.lower_bound(1), *fset.lower_bound(1));
  EXPECT_EQ(*st.lower_bound(3), *fset.lower_bound(3));
  EXPECT_EQ(*st.lower_bound(4), *fset.lower_bound(4));
  EXPECT_EQ(*st.lower_bound(5), *fset.lower_bound(5));
}

TEST(Multiset, Upper_bound) {
  s21::multiset<int> st{7, 1, 3, 3, 4, 4, 4, 3, 5};
  std::multiset<int> fset{7, 1, 3, 3, 4, 4, 4, 3, 5};
  EXPECT_EQ(*st.upper_bound(1), *fset.upper_bound(1));
  EXPECT_EQ(*st.upper_bound(3), *fset.upper_bound(3));
  EXPECT_EQ(*st.upper_bound(4), *fset.upper_bound(4));
  EXPECT_EQ(*st.upper_bound(5), *fset.upper_bound(5));
}

TEST(Multiset, Count) {
  s21::multiset<int> st{7, 1, 3, 3, 4, 4, 4, 3, 5};
  EXPECT_EQ(st.count(1), 1);
  EXPECT_EQ(st.count(3), 3);
  EXPECT_EQ(st.count(4), 3);
  EXPECT_EQ(st.count(5), 1);
  EXPECT_EQ(st.count(88), 0);
}

TEST(Multiset, Find) {
  s21::multiset<int> st{7, 1, 3, 3, 4, 4, 4, 3, 5};
  s21::multiset<int>::iterator it = st.begin();
  EXPECT_EQ(*st.find(1), *it);
  ++it;
  EXPECT_EQ(*it, *st.find(3));
  ++it;
  ++it;
  ++it;
  EXPECT_EQ(*it, *st.find(4));
}

TEST(Multiset, Contains) {
  s21::multiset<int> st{7, 1, 3, 3, 4, 4, 4, 3, 5};
  EXPECT_TRUE(st.contains(7));
  EXPECT_TRUE(st.contains(4));
  EXPECT_TRUE(st.contains(5));
  EXPECT_TRUE(st.contains(1));
  EXPECT_FALSE(st.contains(888));
}

TEST(Multiset, Emplace) {
  s21::multiset<int> st{7, 4, 9};
  st.emplace(3, 1, 20);
  s21::multiset<int>::iterator it = st.begin();
  EXPECT_EQ(st.size(), 6);
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 3);
  s21::multiset<int>::iterator it_end = st.end();
  --it_end;
  EXPECT_EQ(*it_end, 20);
  --it_end;
  EXPECT_EQ(*it_end, 9);
}
