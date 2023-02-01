#include "s21_matrix.h"

int s21_create_matrix(matrix_t *A, size_t rows, int columns) {
  A->rows = rows;
  A->columns = columns;
  if (rows == 0 || columns <= 0) return ERR_WRONG_MATRIX;

  A->matrix = (float **)malloc(rows * sizeof(float *));
  if (A->matrix == NULL) return ERR_MALLOC;

  for (size_t i = 0; i < rows; i++) {
    A->matrix[i] = (float *)malloc(columns * sizeof(float));
    if (A->matrix[i] == NULL) return ERR_MALLOC;
  }

  return NO_ERROR;
}

int s21_realloc_matrix(matrix_t *A) {
  A->matrix = (float **)realloc(A->matrix, A->rows * 2 * sizeof(float *));
  if (A->matrix == NULL) return ERR_REALLOC;

  for (size_t i = A->rows; i < A->rows * 2; i++) {
    A->matrix[i] = (float *)malloc(A->columns * sizeof(float));
  }
  A->rows *= 2;

  return NO_ERROR;
}

void s21_print_matrix(matrix_t *A) {
  for (size_t i = 0; i < A->vertex; i++) {
    for (int j = 0; j < A->columns; j++) {
      printf("%15f", A->matrix[i][j]);
    }
    printf("\n");
  }
}

void s21_remove_matrix(matrix_t *A) {
  if (A->rows > 0 && A->columns > 0) {
    for (size_t i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
  A->rows = A->columns = A->vertex = 0;
  A->matrix = NULL;
}

// Перемещение модели по осям
void s21_move_model(matrix4x4 *A, float x, float y, float z) {
  A->m[0][3] = x;
  A->m[1][3] = y;
  A->m[2][3] = z;
}

// Поворот модели относительно оси Х
void s21_rotate_x_axis(matrix4x4 *A, float angle) {
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);
  float tmp;
  A->m[1][0] = (tmp = A->m[1][0]) * c + A->m[2][0] * s;
  A->m[2][0] = A->m[2][0] * c - tmp * s;
  A->m[1][1] = (tmp = A->m[1][1]) * c + A->m[2][1] * s;
  A->m[2][1] = A->m[2][1] * c - tmp * s;
  A->m[1][2] = (tmp = A->m[1][2]) * c + A->m[2][2] * s;
  A->m[2][2] = A->m[2][2] * c - tmp * s;
  A->m[1][3] = (tmp = A->m[1][3]) * c + A->m[2][3] * s;
  A->m[2][3] = A->m[2][3] * c - tmp * s;
}

// Поворот модели относительно оси Y
void s21_rotate_y_axis(matrix4x4 *A, float angle) {
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);
  float tmp;
  A->m[2][0] = (tmp = A->m[2][0]) * c + A->m[0][0] * s;
  A->m[0][0] = A->m[0][0] * c - tmp * s;
  A->m[2][1] = (tmp = A->m[2][1]) * c + A->m[0][1] * s;
  A->m[0][1] = A->m[0][1] * c - tmp * s;
  A->m[2][2] = (tmp = A->m[2][2]) * c + A->m[0][2] * s;
  A->m[0][2] = A->m[0][2] * c - tmp * s;
  A->m[2][3] = (tmp = A->m[2][3]) * c + A->m[0][3] * s;
  A->m[0][3] = A->m[0][3] * c - tmp * s;
}

// Поворот модели относительно оси Z
void s21_rotate_z_axis(matrix4x4 *A, float angle) {
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);
  float tmp;
  A->m[0][0] = (tmp = A->m[0][0]) * c + A->m[1][0] * s;
  A->m[1][0] = A->m[1][0] * c - tmp * s;
  A->m[0][1] = (tmp = A->m[0][1]) * c + A->m[1][1] * s;
  A->m[1][1] = A->m[1][1] * c - tmp * s;
  A->m[0][2] = (tmp = A->m[0][2]) * c + A->m[1][2] * s;
  A->m[1][2] = A->m[1][2] * c - tmp * s;
  A->m[0][3] = (tmp = A->m[0][3]) * c + A->m[1][3] * s;
  A->m[1][3] = A->m[1][3] * c - tmp * s;
}

// Масштабирование модели
void s21_scale_model(matrix4x4 *A, float scale) {
  scale *= A->m[3][3];
  if (fabs(scale) >= EPS) {
    A->m[0][0] = A->m[0][0] * scale;
    A->m[1][1] = A->m[1][1] * scale;
    A->m[2][2] = A->m[2][2] * scale;
    A->m[3][3] = scale;
  }
}

void s21_init_matrix4x4(matrix4x4 *A, float value) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      A->m[i][j] = value;
    }
  }
}
