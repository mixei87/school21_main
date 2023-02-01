#ifndef S21_MATRIX_H_
#define S21_MATRIX_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_error.h"
#define EPS 1E-6

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

typedef struct matrix_struct {
  float **matrix;
  size_t vertex;
  size_t rows;
  int columns;
} matrix_t;

typedef struct matrix4x4 {
  float m[4][4];
} matrix4x4;

int s21_create_matrix(matrix_t *A, size_t rows, int columns);
int s21_realloc_matrix(matrix_t *input_matrix);
void s21_print_matrix(matrix_t *A);
void s21_remove_matrix(matrix_t *A);
void s21_move_model(matrix4x4 *A, float x, float y, float z);
void s21_rotate_x_axis(matrix4x4 *A, float angle);
void s21_rotate_y_axis(matrix4x4 *A, float angle);
void s21_rotate_z_axis(matrix4x4 *A, float angle);
void s21_scale_model(matrix4x4 *A, float scale);
void s21_init_matrix4x4(matrix4x4 *A, float value);

#endif  // S21_MATRIX_H_
