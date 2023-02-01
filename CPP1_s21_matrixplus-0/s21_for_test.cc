#include "s21_matrix_oop.h"

// row from 0
void S21Matrix::SetMatrix(int row) {
  std::ifstream file("test_matrix.txt");
  if (!file) throw std::invalid_argument(err_.error_[6].second);
  for (int i = 0; !file.eof() && i < row; i++)
    file.ignore(std::numeric_limits<int>::max(), '\n');
  if (file.eof()) throw std::invalid_argument(err_.error_[7].second);

  std::string buf;
  std::getline(file, buf);
  for (int i = 0; !buf.empty() && i < rows_; i++) {
    for (int j = 0; !buf.empty() && j < cols_; j++) {
      std::size_t size;
      double current_number;
      current_number = std::stod(buf, &size);
      if (!size) throw std::invalid_argument(err_.error_[8].second);
      buf = buf.substr(size);
      matrix_[i][j] = current_number;
    }
  }
}

double **S21Matrix::GetPtr() const { return this->matrix_; }
