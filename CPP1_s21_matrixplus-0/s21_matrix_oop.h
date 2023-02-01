#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "s21_error.h"

class S21Matrix {
 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other);
  ~S21Matrix();

  bool EqMatrix(const S21Matrix &other) const;
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  double **GetPtr() const;
  int getRows() const;
  int getCols() const;
  void setRows(int rows);
  void setCols(int cols);

  void SetMatrix(int row);

  S21Matrix operator+(const S21Matrix &other) const;
  S21Matrix operator-(const S21Matrix &other) const;
  S21Matrix operator*(const S21Matrix &other) const;
  S21Matrix operator*(const double num) const;
  S21Matrix &operator=(const S21Matrix &other);
  bool operator==(const S21Matrix &other) const;
  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix &operator*=(const S21Matrix &other);
  S21Matrix &operator*=(const double num);
  double &operator()(const int i, const int j) const;

 private:
  int rows_, cols_;
  double **matrix_;
  const double kEpsilon_ = 1E-7;
  std::string filename_matrix_ = "test_matrix.txt";
  void InitMatrix();
  void RemoveMatrix();
  void CopyMatrix(const S21Matrix &other);
  void CopyMatrixForSetters(const S21Matrix &other, const int &new_rows,
                            const int &new_cols, const int &rows_copy,
                            const int &cols_copy);
  void CheckSizeMatrixes(const S21Matrix &other) const;
  void IsMatrixSquare() const;
  double SetMinor(S21Matrix A, int i, int j) const;
  Error err_;
};

#endif  // SRC_S21_MATRIX_OOP_H_
