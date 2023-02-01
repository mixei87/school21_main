#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : S21Matrix(3, 4) {}

S21Matrix::S21Matrix(int rows, int cols) : matrix_{nullptr} {
  if (rows <= 0 || cols <= 0)
    throw std::invalid_argument(err_.error_[5].second);
  rows_ = rows, cols_ = cols;
  InitMatrix();
}

S21Matrix::S21Matrix(const S21Matrix &other) : matrix_{nullptr} {
  CopyMatrix(other);
}

S21Matrix::S21Matrix(S21Matrix &&other) : rows_{0}, cols_{0}, matrix_{nullptr} {
  if (this != &other) {
    std::swap(this->matrix_, other.matrix_);
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
  }
}

S21Matrix::~S21Matrix() { RemoveMatrix(); }
