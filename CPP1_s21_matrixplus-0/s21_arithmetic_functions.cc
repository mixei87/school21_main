#include "s21_matrix_oop.h"

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  bool equal = (rows_ == other.rows_) && (cols_ == other.cols_);
  for (int i = 0; equal && i < rows_; i++)
    for (int j = 0; equal && j < cols_; j++)
      equal = equal && (fabs(matrix_[i][j] - other.matrix_[i][j]) < kEpsilon_);
  return equal;
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  CheckSizeMatrixes(other);
  if (!other.matrix_) throw std::invalid_argument(err_.error_[2].second);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] += other.matrix_[i][j];
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  CheckSizeMatrixes(other);
  if (!other.matrix_) throw std::invalid_argument(err_.error_[2].second);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] -= other.matrix_[i][j];
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) throw std::invalid_argument(err_.error_[3].second);
  if (!other.matrix_) throw std::invalid_argument(err_.error_[2].second);
  S21Matrix first = std::move(*this);
  rows_ = first.rows_;
  cols_ = other.cols_;
  InitMatrix();

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      for (int k = 0; k < first.cols_; k++) {
        matrix_[i][j] += first.matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix transpose_matrix(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      transpose_matrix.matrix_[j][i] = matrix_[i][j];
    }
  }
  return transpose_matrix;
}

S21Matrix S21Matrix::CalcComplements() const {
  IsMatrixSquare();
  S21Matrix A(rows_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < rows_; j++) {
      A.matrix_[i][j] = SetMinor(*this, i, j) * std::pow(-1, i + j);
    }
  }
  return A;
}

double S21Matrix::SetMinor(S21Matrix A, int i, int j) const {
  double res;
  int old_size = A.rows_;
  if (old_size > 1) {
    int new_size = old_size - 1;
    S21Matrix B(new_size, new_size);
    for (int k = 0, I = 0; k < new_size; k++, I++) {
      if (I == i) I++;
      for (int p = 0, J = 0; p < new_size; p++, J++) {
        if (J == j) J++;
        B.matrix_[k][p] = A.matrix_[I][J];
      }
    }
    if (new_size == 1) {
      res = B.matrix_[0][0];
    } else {
      res = 0;
      for (int p = 0; p < new_size; p++) {
        res += SetMinor(B, 0, p) * B.matrix_[0][p] * pow(-1, p);
      }
    }
  } else {
    res = A.matrix_[0][0];
  }
  return res;
}

double S21Matrix::Determinant() const {
  IsMatrixSquare();
  double res;
  if (rows_ == 1) {
    res = matrix_[0][0];
  } else {
    res = 0;
    S21Matrix A = CalcComplements();
    for (int j = 0; j < rows_; j++) {
      res += matrix_[0][j] * A.matrix_[0][j];
    }
  }
  return res;
}

S21Matrix S21Matrix::InverseMatrix() const {
  IsMatrixSquare();
  double det = Determinant();
  if (fabs(det) < kEpsilon_) throw std::invalid_argument(err_.error_[4].second);
  S21Matrix A = CalcComplements().Transpose();
  A.MulNumber(1.0 / det);
  return A;
}
