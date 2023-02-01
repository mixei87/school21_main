#include "s21_matrix_oop.h"

int S21Matrix::getRows() const { return rows_; }

int S21Matrix::getCols() const { return cols_; }

void S21Matrix::setRows(int rows) {
  if (rows != rows_) {
    if (rows <= 0) throw std::invalid_argument(err_.error_[10].second);
    int rows_copy = rows < rows_ ? rows : rows_;
    int cols_copy = cols_;
    S21Matrix tmp = std::move(*this);
    CopyMatrixForSetters(tmp, rows, cols_copy, rows_copy, cols_copy);
  }
}
void S21Matrix::setCols(int cols) {
  if (cols != cols_) {
    if (cols <= 0) throw std::invalid_argument(err_.error_[11].second);
    int rows_copy = rows_;
    int cols_copy = cols < cols_ ? cols : cols_;
    S21Matrix tmp = std::move(*this);
    CopyMatrixForSetters(tmp, rows_copy, cols, rows_copy, cols_copy);
  }
}
