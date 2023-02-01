// gcc tests.c parser.c s21_matrix.c -lm -L/usr/local/lib -lcheck -o test &&
// ./test

#include <check.h>
#include <math.h>

#include "parser.h"
#include "s21_matrix.h"

START_TEST(parser_test_2) {
  model_3d model;
  int error = init_model(&model);
  char *filename = "objects/cat.obj";
  int status = parser_data(&model, filename);

  ck_assert_int_eq(0, status);
  ck_assert_int_eq(0, error);

  ck_assert_uint_eq(35290, model.vertex_coord.vertex);
  ck_assert_uint_eq(35288, model.num_poligon);

  ck_assert_float_eq(0.5544, model.vertex_coord.matrix[5][0]);
  ck_assert_float_eq(-21.8847, model.vertex_coord.matrix[6][1]);
  ck_assert_float_eq(31.4781, model.vertex_coord.matrix[7][2]);
  ck_assert_float_eq(31.7040, model.vertex_coord.matrix[26][2]);
  ck_assert_float_eq(-21.6008, model.vertex_coord.matrix[27][1]);
  ck_assert_float_eq(0.3267, model.vertex_coord.matrix[28][0]);
  ck_assert_float_eq(15.7852, model.vertex_coord.matrix[35281][2]);
  ck_assert_float_eq(-15.6849, model.vertex_coord.matrix[35280][1]);
  ck_assert_float_eq(0.7217, model.vertex_coord.matrix[35279][0]);

  ck_assert_uint_eq(4, model.poligon[7].num_poligon_vertexes);
  ck_assert_uint_eq(5, model.poligon[7].vertex_array[0]);
  ck_assert_uint_eq(4, model.poligon[8].num_poligon_vertexes);
  ck_assert_uint_eq(16, model.poligon[8].vertex_array[1]);
  ck_assert_uint_eq(4, model.poligon[9].num_poligon_vertexes);
  ck_assert_uint_eq(20, model.poligon[9].vertex_array[2]);
  ck_assert_uint_eq(4, model.poligon[10].num_poligon_vertexes);
  ck_assert_uint_eq(13, model.poligon[10].vertex_array[3]);

  ck_assert_uint_eq(4, model.poligon[35285].num_poligon_vertexes);
  ck_assert_uint_eq(35283, model.poligon[35285].vertex_array[3]);
  ck_assert_uint_eq(4, model.poligon[35284].num_poligon_vertexes);
  ck_assert_uint_eq(35278, model.poligon[35284].vertex_array[2]);
  ck_assert_uint_eq(4, model.poligon[35283].num_poligon_vertexes);
  ck_assert_uint_eq(35288, model.poligon[35283].vertex_array[1]);
  ck_assert_uint_eq(4, model.poligon[35282].num_poligon_vertexes);
  ck_assert_uint_eq(29838, model.poligon[35282].vertex_array[0]);

  free_memory(&model);
}
END_TEST

START_TEST(affine_test_1) {
  matrix4x4 matr;
  memset(matr.m, 0, sizeof(matr.m));

  s21_move_model(&matr, 3.5, 0, 0);
  ck_assert_float_eq(0.0, matr.m[0][0]);
  ck_assert_float_eq(3.5, matr.m[0][3]);
  ck_assert_float_eq(0.0, matr.m[1][1]);
  ck_assert_float_eq(0.0, matr.m[2][2]);
  ck_assert_float_eq(0.0, matr.m[3][3]);

  s21_move_model(&matr, 0, -18.0, 0);
  ck_assert_float_eq(0.0, matr.m[0][0]);
  ck_assert_float_eq(0.0, matr.m[1][1]);
  ck_assert_float_eq(-18.0, matr.m[1][3]);
  ck_assert_float_eq(0.0, matr.m[2][2]);
  ck_assert_float_eq(0.0, matr.m[3][3]);

  s21_move_model(&matr, 0, 0, 0.5);
  ck_assert_float_eq(0.0, matr.m[0][0]);
  ck_assert_float_eq(0.0, matr.m[1][1]);
  ck_assert_float_eq(0.0, matr.m[2][2]);
  ck_assert_float_eq(0.5, matr.m[2][3]);
  ck_assert_float_eq(0.0, matr.m[3][3]);
}
END_TEST

START_TEST(affine_test_2) {
  matrix4x4 matr_sc;
  s21_init_matrix4x4(&matr_sc, 1.0);

  s21_scale_model(&matr_sc, 4.5);
  ck_assert_float_eq(4.5, matr_sc.m[0][0]);
  ck_assert_float_eq(1.0, matr_sc.m[0][2]);
  ck_assert_float_eq(4.5, matr_sc.m[1][1]);
  ck_assert_float_eq(4.5, matr_sc.m[2][2]);
  ck_assert_float_eq(1.0, matr_sc.m[2][3]);
  ck_assert_float_eq(4.5, matr_sc.m[3][3]);

  s21_scale_model(&matr_sc, -2.0);
  ck_assert_float_eq(-40.5, matr_sc.m[0][0]);
  ck_assert_float_eq(1.0, matr_sc.m[0][2]);
  ck_assert_float_eq(-40.5, matr_sc.m[1][1]);
  ck_assert_float_eq(-40.5, matr_sc.m[2][2]);
  ck_assert_float_eq(1.0, matr_sc.m[2][3]);
  ck_assert_float_eq(-9.0, matr_sc.m[3][3]);

  s21_scale_model(&matr_sc, 0.0);
  ck_assert_float_eq(-40.5, matr_sc.m[0][0]);
  ck_assert_float_eq(1.0, matr_sc.m[0][2]);
  ck_assert_float_eq(-40.5, matr_sc.m[1][1]);
  ck_assert_float_eq(-40.5, matr_sc.m[2][2]);
  ck_assert_float_eq(1.0, matr_sc.m[2][3]);
  ck_assert_float_eq(-9.0, matr_sc.m[3][3]);
}
END_TEST

START_TEST(affine_test_3) {
  matrix4x4 matr;
  s21_init_matrix4x4(&matr, 1.0);

  float angle = 30.0;
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);

  float m00 = 1.0;
  float m10 = matr.m[1][0] * c + matr.m[2][0] * s;
  float m11 = matr.m[1][1] * c + matr.m[2][1] * s;
  float m22 = matr.m[2][2] * c - matr.m[1][2] * s;
  float m33 = 1.0;

  s21_rotate_x_axis(&matr, angle);

  ck_assert_float_eq(m00, matr.m[0][0]);
  ck_assert_float_eq(m10, matr.m[1][0]);
  ck_assert_float_eq(m11, matr.m[1][1]);
  ck_assert_float_eq(m22, matr.m[2][2]);
  ck_assert_float_eq(m33, matr.m[3][3]);
}
END_TEST

START_TEST(affine_test_4) {
  matrix4x4 matr;
  s21_init_matrix4x4(&matr, 1.0);

  float angle = -45.0;
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);

  float m00 = matr.m[0][0] * c - matr.m[2][0] * s;
  float m03 = matr.m[0][3] * c - matr.m[2][3] * s;
  float m11 = 1.0;
  float m22 = matr.m[2][2] * c + matr.m[0][2] * s;
  float m33 = 1.0;

  s21_rotate_y_axis(&matr, angle);

  ck_assert_float_eq(m00, matr.m[0][0]);
  ck_assert_float_eq(m03, matr.m[0][3]);
  ck_assert_float_eq(m11, matr.m[1][1]);
  ck_assert_float_eq(m22, matr.m[2][2]);
  ck_assert_float_eq(m33, matr.m[3][3]);
}
END_TEST

START_TEST(affine_test_5) {
  matrix4x4 matr;
  s21_init_matrix4x4(&matr, 1.0);

  float angle = 95.0;
  float a = M_PI * angle / 180.0f;
  float c = cosf(a);
  float s = sinf(a);

  float m00 = matr.m[0][0] * c + matr.m[1][0] * s;
  float m02 = matr.m[0][2] * c + matr.m[1][2] * s;
  float m11 = matr.m[1][1] * c - matr.m[0][1] * s;
  float m22 = 1.0;
  float m33 = 1.0;

  s21_rotate_z_axis(&matr, angle);

  ck_assert_float_eq(m00, matr.m[0][0]);
  ck_assert_float_eq(m02, matr.m[0][2]);
  ck_assert_float_eq(m11, matr.m[1][1]);
  ck_assert_float_eq(m22, matr.m[2][2]);
  ck_assert_float_eq(m33, matr.m[3][3]);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, parser_test_2);
  tcase_add_test(tc1_1, affine_test_1);
  tcase_add_test(tc1_1, affine_test_2);
  tcase_add_test(tc1_1, affine_test_3);
  tcase_add_test(tc1_1, affine_test_4);
  tcase_add_test(tc1_1, affine_test_5);

  srunner_run_all(sr, CK_ENV);

  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
