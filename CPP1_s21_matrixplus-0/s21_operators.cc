#include "s21_matrix_oop.h"

S21Matrix S21Matrix::operator+(const S21Matrix &other) const {
  S21Matrix sum(*this);
  sum.SumMatrix(other);
  return sum;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) const {
  S21Matrix sub(*this);
  sub.SubMatrix(other);
  return sub;
}

S21Matrix S21Matrix::operator*(const double num) const {
  S21Matrix mul(*this);
  mul.MulNumber(num);
  return mul;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21Matrix mul(*this);
  mul.MulMatrix(other);
  return mul;
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  CopyMatrix(other);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

double &S21Matrix::operator()(const int i, const int j) const {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
    throw std::invalid_argument(err_.error_[9].second);
  return matrix_[i][j];
}
