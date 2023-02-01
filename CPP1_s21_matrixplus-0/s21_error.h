#ifndef SRC_S21_ERROR_H_
#define SRC_S21_ERROR_H_

#include <string>
#include <utility>
#include <vector>

class Error {
 public:
  std::vector<std::pair<int, std::string>> error_{
      std::make_pair(0, "Sizes of matrixes are not equal"),
      std::make_pair(1, "Matrix is not square"),
      std::make_pair(2, "Not correct matrix"),
      std::make_pair(
          3, "Number of cols 1st matrix NOT equal number of rows 2nd matrix"),
      std::make_pair(4, "Determinant equal 0"),
      std::make_pair(5, "Incorrect size of matrix: need rows > 0 && cols >0"),
      std::make_pair(6, "Can't open file"),
      std::make_pair(7, "This Matrix isn't in file"),
      std::make_pair(8, "Bad content of file: not number"),
      std::make_pair(9, "Index out of range"),
      std::make_pair(10, "Incorrect size of matrix: need rows > 0"),
      std::make_pair(11, "Incorrect size of matrix: need cols > 0")};
};
#endif  // SRC_S21_ERROR_H_
