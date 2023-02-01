#include <gtest/gtest.h>

#include "s21_error.h"
#include "s21_matrix_oop.h"

TEST(DefaultConstructor, test1) {
  S21Matrix a;
  const int m = 3, n = 4;
  ASSERT_EQ(a.getRows(), m);
  ASSERT_EQ(a.getCols(), n);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) ASSERT_EQ(a(i, j), 0);
}
TEST(DefaultConstructor, test2) {
  S21Matrix a;
  S21Matrix b{3, 3};
  ASSERT_FALSE(a.EqMatrix(b));
}
TEST(DefaultConstructor, test3) {
  S21Matrix a;
  S21Matrix b{3, 4};
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(ConstructorWithParams, test1) {
  const int m = 2, n = 5;
  S21Matrix a{m, n};
  ASSERT_EQ(a.getRows(), m);
  ASSERT_EQ(a.getCols(), n);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) ASSERT_EQ(a(i, j), 0);
}
TEST(ConstructorWithParamsDeath, test1) {
  const int m = -1, n = 5;
  ASSERT_ANY_THROW({ S21Matrix a(m, n); });
}

TEST(ConstructorWithParamsDeath, test2) {
  const int m = 3, n = 0;
  ASSERT_ANY_THROW({ S21Matrix a(m, n); });
}
TEST(ConstructorCopy, test1) {
  S21Matrix a;
  a.SetMatrix(0);
  S21Matrix b(a);
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(ConstructorMove, test1) {
  S21Matrix a(std::move(a));
  const int m = 0, n = 0;
  ASSERT_EQ(a.getRows(), m);
  ASSERT_EQ(a.getCols(), n);
  ASSERT_TRUE(a.GetPtr() == nullptr);
}
TEST(ConstructorMove, test2) {
  const int mA = 6, nA = 3, mB = 0, nB = 0;
  S21Matrix a{mA, nA};
  S21Matrix b(std::move(a));
  ASSERT_EQ(a.getRows(), mB);
  ASSERT_EQ(a.getCols(), nB);
  ASSERT_TRUE(a.GetPtr() == nullptr);
  ASSERT_EQ(b.getRows(), mA);
  ASSERT_EQ(b.getCols(), nA);
}
TEST(EqMatrix, test1) {
  S21Matrix a;
  a.SetMatrix(0);
  S21Matrix b;
  b.SetMatrix(1);
  ASSERT_FALSE(a.EqMatrix(b));
}
TEST(EqMatrix, test2) {
  S21Matrix a;
  a.SetMatrix(2);
  S21Matrix b(a);
  ASSERT_TRUE(a.EqMatrix(b));
  b(2, 3) = -9.8;
  ASSERT_FALSE(a.EqMatrix(b));
}
TEST(SumMatrix, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(4);
  S21Matrix b{m, n};
  b.SetMatrix(5);
  S21Matrix c{m, n};
  c.SetMatrix(6);
  a.SumMatrix(b);
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(SumMatrix, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(4);
  S21Matrix b{m, n};
  b.SetMatrix(5);
  S21Matrix c{m, n};
  c.SetMatrix(2);
  a.SumMatrix(b);
  ASSERT_FALSE(a.EqMatrix(c));
}
TEST(SumMatrix, test3) {
  const int m = 6, n = 6;
  S21Matrix a{m, n};
  a.SetMatrix(8);
  S21Matrix b{m, n};
  b.SetMatrix(9);
  S21Matrix c{m, n};
  c.SetMatrix(10);
  a.SumMatrix(b);
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(SumMatrix, test4) {
  const int m = 6, n = 6;
  S21Matrix a{m, n};
  a.SetMatrix(8);
  S21Matrix b{m, n};
  b.SetMatrix(9);
  S21Matrix c{m, n};
  c.SetMatrix(4);
  a.SumMatrix(b);
  ASSERT_FALSE(a.EqMatrix(c));
}
TEST(SubMatrix, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(12);
  S21Matrix b{m, n};
  b.SetMatrix(13);
  S21Matrix c{m, n};
  c.SetMatrix(14);
  a.SubMatrix(b);
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(SubMatrix, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(12);
  S21Matrix b{m, n};
  b.SetMatrix(13);
  S21Matrix c{m, n};
  c.SetMatrix(0);
  a.SubMatrix(b);
  ASSERT_FALSE(a.EqMatrix(c));
}
TEST(SubMatrix, test3) {
  const int m = 6, n = 6;
  S21Matrix a{m, n};
  a.SetMatrix(16);
  S21Matrix b{m, n};
  b.SetMatrix(17);
  S21Matrix c{m, n};
  c.SetMatrix(18);
  a.SubMatrix(b);
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(SubMatrix, test4) {
  const int m = 6, n = 6;
  S21Matrix a{m, n};
  a.SetMatrix(16);
  S21Matrix b{m, n};
  b.SetMatrix(18);
  S21Matrix c{m, n};
  c.SetMatrix(11);
  a.SubMatrix(b);
  ASSERT_FALSE(a.EqMatrix(c));
}
TEST(SubMatrixDeath, test1) {
  const int m1 = 3, n1 = 6;
  const int m2 = 6, n2 = 6;
  S21Matrix a(m1, n1);
  a.SetMatrix(16);
  S21Matrix b(m2, n2);
  b.SetMatrix(18);
  ASSERT_ANY_THROW(a.SubMatrix(b));
}
TEST(MulNumber, test1) {
  const int m = 3, n = 3;
  const double x = 1.765;
  S21Matrix a{m, n};
  a.SetMatrix(20);
  S21Matrix b{m, n};
  b.SetMatrix(21);
  a.MulNumber(x);
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(MulNumber, test2) {
  const int m = 3, n = 3;
  const double x = -1.4765;
  S21Matrix a{m, n};
  a.SetMatrix(20);
  S21Matrix b{m, n};
  b.SetMatrix(21);
  a.MulNumber(x);
  ASSERT_FALSE(a.EqMatrix(b));
}
TEST(MulMatrix, test1) {
  const int m1 = 5, n1 = 2;
  const int m2 = 2, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(23);
  S21Matrix b(m2, n2);
  b.SetMatrix(24);
  S21Matrix c(m1, n2);
  c.SetMatrix(25);
  a.MulMatrix(b);
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(MulMatrix, test2) {
  const int m1 = 5, n1 = 2;
  const int m2 = 2, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(20);
  S21Matrix b(m2, n2);
  b.SetMatrix(21);
  S21Matrix c(m1, n2);
  c.SetMatrix(18);
  a.MulMatrix(b);
  ASSERT_FALSE(a.EqMatrix(c));
}
TEST(MulMatrixDeath, test1) {
  const int m1 = 5, n1 = 2;
  const int m2 = 3, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(20);
  S21Matrix b(m2, n2);
  b.SetMatrix(21);
  ASSERT_ANY_THROW(a.MulMatrix(b));
}
TEST(Transpose, test1) {
  const int m1 = 3, n1 = 3;
  const int m2 = 3, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(27);
  S21Matrix b(m2, n2);
  b.SetMatrix(28);
  ASSERT_TRUE(a.Transpose().EqMatrix(b));
}
TEST(Transpose, test2) {
  const int m1 = 4, n1 = 2;
  const int m2 = 2, n2 = 4;
  S21Matrix a(m1, n1);
  a.SetMatrix(30);
  S21Matrix b(m2, n2);
  b.SetMatrix(31);
  ASSERT_TRUE(a.Transpose().EqMatrix(b));
}
TEST(CalcComplements, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(33);
  S21Matrix b{m, n};
  b.SetMatrix(34);
  ASSERT_TRUE(a.CalcComplements().EqMatrix(b));
}
TEST(CalcComplements, test2) {
  const int m = 1, n = 1;
  S21Matrix a{m, n};
  a(0, 0) = 34.908;
  ASSERT_TRUE(a.CalcComplements().EqMatrix(a));
}
TEST(CalcComplementsDeath, test1) {
  const int m = 3, n = 2;
  S21Matrix a{m, n};
  ASSERT_ANY_THROW(a.CalcComplements());
}
TEST(Determinant, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(36);
  ASSERT_DOUBLE_EQ(a.Determinant(), 7.470044912);
}
TEST(Determinant, test2) {
  const int m = 1, n = 1;
  S21Matrix a{m, n};
  a(0, 0) = -934.23441;
  ASSERT_DOUBLE_EQ(a.Determinant(), -934.23441);
}
TEST(DeterminantDeath, test1) {
  const int m = 3, n = 8;
  S21Matrix a{m, n};
  ASSERT_ANY_THROW(a.Determinant());
}
TEST(Inverse, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(38);
  S21Matrix b{m, n};
  b.SetMatrix(39);
  ASSERT_TRUE(a.InverseMatrix().EqMatrix(b));
}
TEST(InverseDeath, test1) {
  S21Matrix a;
  ASSERT_ANY_THROW(a.InverseMatrix());
}
TEST(InverseDeath, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  ASSERT_ANY_THROW(a.InverseMatrix());
}
TEST(OperatorPlus, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(4);
  S21Matrix b{m, n};
  b.SetMatrix(5);
  S21Matrix c{m, n};
  c.SetMatrix(6);
  ASSERT_TRUE((a + b).EqMatrix(c));
}
TEST(OperatorPlus, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(4);
  S21Matrix b{m, n};
  b.SetMatrix(5);
  S21Matrix c{m, n};
  S21Matrix d{m, n};
  d.SetMatrix(6);
  ASSERT_TRUE((a + b + c).EqMatrix(d));
}
TEST(OperatorPlus, test3) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(4);
  S21Matrix b{m, n};
  b.SetMatrix(5);
  S21Matrix c{m, n};
  c.SetMatrix(6);
  a = a + b;
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(OperatorMinus, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(12);
  S21Matrix b{m, n};
  b.SetMatrix(13);
  S21Matrix c{m, n};
  c.SetMatrix(14);
  ASSERT_TRUE((a - b).EqMatrix(c));
}
TEST(OperatorMinus, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(12);
  S21Matrix b{m, n};
  b.SetMatrix(13);
  S21Matrix c{m, n};
  S21Matrix d{m, n};
  d.SetMatrix(14);
  ASSERT_TRUE((a - b - c).EqMatrix(d));
}
TEST(OperatorMulNumber, test1) {
  const int m = 3, n = 3;
  const double x = 1.765;
  S21Matrix a{m, n};
  a.SetMatrix(20);
  S21Matrix b{m, n};
  b.SetMatrix(21);
  ASSERT_TRUE((a * x).EqMatrix(b));
}
TEST(OperatorMulNumber, test2) {
  const int m = 3, n = 3;
  const double x = -1.4765;
  S21Matrix a{m, n};
  a.SetMatrix(20);
  S21Matrix b{m, n};
  b.SetMatrix(21);
  ASSERT_FALSE((a * x).EqMatrix(b));
}
TEST(OperatorMulMatrix, test1) {
  const int m1 = 5, n1 = 2;
  const int m2 = 2, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(23);
  S21Matrix b(m2, n2);
  b.SetMatrix(24);
  S21Matrix c(m1, n2);
  c.SetMatrix(25);
  ASSERT_TRUE((a * b).EqMatrix(c));
}
TEST(OperatorMulMatrix, test2) {
  const int m1 = 5, n1 = 2;
  const int m2 = 2, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(20);
  S21Matrix b(m2, n2);
  b.SetMatrix(21);
  S21Matrix c(m1, n2);
  c.SetMatrix(18);
  ASSERT_FALSE((a * b).EqMatrix(c));
}
TEST(OperatorMulMatrixDeath, test1) {
  const int m1 = 5, n1 = 2;
  const int m2 = 3, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(20);
  S21Matrix b(m2, n2);
  b.SetMatrix(21);
  ASSERT_ANY_THROW(a * b);
}
TEST(OperatorAssert, test1) {
  const int m = 5, n = 2;
  S21Matrix a{m, n};
  a.SetMatrix(23);
  S21Matrix b;
  b = a;
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(OperatorAssert, test2) {
  const int m = 5, n = 2;
  S21Matrix a{m, n};
  a.SetMatrix(23);
  S21Matrix b, c;
  c = b = a;
  ASSERT_TRUE(a.EqMatrix(b));
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(OperatorEqual, test1) {
  const int m = 5, n = 2;
  S21Matrix a{m, n};
  a.SetMatrix(23);
  S21Matrix b(a);
  ASSERT_TRUE(a == b);
}
TEST(OperatorEqual, test2) {
  const int m = 5, n = 2;
  S21Matrix a{m, n};
  a.SetMatrix(23);
  S21Matrix b(a);
  b(0, 0) = -999.888777666;
  ASSERT_FALSE(a == b);
}
TEST(OperatorPlusEqual, test1) {
  const int m = 6, n = 6;
  S21Matrix a{m, n};
  a.SetMatrix(8);
  S21Matrix b{m, n};
  b.SetMatrix(9);
  S21Matrix c{m, n};
  c.SetMatrix(10);
  a += b;
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(OperatorMinusEqual, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(12);
  S21Matrix b{m, n};
  b.SetMatrix(13);
  S21Matrix c{m, n};
  c.SetMatrix(14);
  a -= b;
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(OperatorMulEqualNumber, test1) {
  const int m = 3, n = 3;
  const double x = 1.765;
  S21Matrix a{m, n};
  a.SetMatrix(20);
  S21Matrix b{m, n};
  b.SetMatrix(21);
  a *= x;
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(OperatorMulEqualMatrix, test1) {
  const int m1 = 5, n1 = 2;
  const int m2 = 2, n2 = 3;
  S21Matrix a(m1, n1);
  a.SetMatrix(23);
  S21Matrix b(m2, n2);
  b.SetMatrix(24);
  S21Matrix c(m1, n2);
  c.SetMatrix(25);
  a *= b;
  ASSERT_TRUE(a.EqMatrix(c));
}
TEST(Index, test1) {
  const int m = 3, n = 3;
  const double x = 1.592;
  S21Matrix a{m, n};
  a.SetMatrix(38);
  ASSERT_DOUBLE_EQ(x, a(0, 0));
}
TEST(Index, test2) {
  const int m = 3, n = 3;
  const double x = 777.77;
  S21Matrix a{m, n};
  a.SetMatrix(38);
  a(0, 0) = x;
  ASSERT_DOUBLE_EQ(x, a(0, 0));
}
TEST(IndexDeath, test1) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(38);
  ASSERT_ANY_THROW(a(0, 3));
}
TEST(IndexDeath, test2) {
  const int m = 3, n = 3;
  S21Matrix a{m, n};
  a.SetMatrix(38);
  ASSERT_ANY_THROW(a(3, -1));
}
TEST(SetRows, test1) {
  const int n = 3;
  int m = 3;
  S21Matrix a{m, n};
  a.SetMatrix(41);
  m = 1;
  a.setRows(m);
  S21Matrix b{m, n};
  b.SetMatrix(42);
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(SetRows, test2) {
  const int n = 3;
  int m = 1;
  S21Matrix a{m, n};
  a.SetMatrix(44);
  m = 3;
  a.setRows(m);
  S21Matrix b{m, n};
  b.SetMatrix(45);
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(SetCols, test1) {
  const int m = 2;
  int n = 4;
  S21Matrix a{m, n};
  a.SetMatrix(47);
  n = 3;
  a.setCols(n);
  S21Matrix b{m, n};
  b.SetMatrix(48);
  ASSERT_TRUE(a.EqMatrix(b));
}
TEST(SetCols, test2) {
  const int m = 2;
  int n = 4;
  S21Matrix a{m, n};
  a.SetMatrix(50);
  n = 5;
  a.setCols(n);
  S21Matrix b{m, n};
  b.SetMatrix(51);
  ASSERT_TRUE(a.EqMatrix(b));
}
int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
