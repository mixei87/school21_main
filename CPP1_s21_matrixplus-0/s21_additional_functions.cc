#include "s21_matrix_oop.h"

void S21Matrix::InitMatrix() {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; i++) matrix_[i] = new double[cols_]{};
}

void S21Matrix::RemoveMatrix() {
  if (matrix_) {
    for (int i = 0; i < rows_ && matrix_[i]; i++) {
      delete[] matrix_[i];
      matrix_[i] = nullptr;
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

void S21Matrix::CopyMatrix(const S21Matrix &other) {
  RemoveMatrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  InitMatrix();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}
void S21Matrix::CopyMatrixForSetters(const S21Matrix &other,
                                     const int &new_rows, const int &new_cols,
                                     const int &rows_copy,
                                     const int &cols_copy) {
  RemoveMatrix();
  rows_ = new_rows;
  cols_ = new_cols;
  InitMatrix();
  for (int i = 0; i < rows_copy; i++) {
    for (int j = 0; j < cols_copy; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

void S21Matrix::CheckSizeMatrixes(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument(err_.error_[0].second);
}

void S21Matrix::IsMatrixSquare() const {
  if (rows_ != cols_) throw std::invalid_argument(err_.error_[1].second);
}
