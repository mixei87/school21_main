#ifndef PARSER_H_
#define PARSER_H_

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_error.h"
#include "s21_matrix.h"

#define VERTEX_NUM 3
#define BEGIN_NUM_VERTEX 10
#define BEGIN_NUM_POLIGON 10

typedef struct poligon {
  long long *vertex_array;
  size_t num_poligon_vertexes;
} poligon_t;

typedef struct model {
  matrix_t vertex_coord;
  poligon_t *poligon;
  size_t num_poligon;
  size_t alloc_poligon;
} model_3d;

int parser_data(model_3d *model, char *filename);
int read_data(FILE *file, model_3d *model);
int parse_vertex(char *line, model_3d *model);
int parse_facet(char *line, model_3d *model);
int count_facet_vertex(char *line);
size_t parse_token(char *token);
void free_memory(model_3d *model);
int init_model(model_3d *model);
int add_index_to_facet(model_3d *model, size_t index, size_t cur_vertex);
int init_poligon(model_3d *model, size_t index);
// void debug_print(model_3d model);

#endif  // PARSER_H_
