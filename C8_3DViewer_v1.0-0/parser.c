#include "parser.h"

int init_model(model_3d *model) {
  int error = NO_ERROR;
  model->vertex_coord.vertex = 0;
  model->num_poligon = 0;
  error =
      s21_create_matrix(&(model->vertex_coord), BEGIN_NUM_VERTEX, VERTEX_NUM);
  if (error != NO_ERROR) return error;
  model->alloc_poligon = BEGIN_NUM_POLIGON;

  error = init_poligon(model, 0);
  return error;
}

int init_poligon(model_3d *model, size_t index) {
  if (model->alloc_poligon > 0) {
    if (index == 0) {
      model->poligon =
          (poligon_t *)malloc(model->alloc_poligon * sizeof(poligon_t));
      if (model->poligon == NULL) return ERR_MALLOC;
    } else {
      model->poligon = (poligon_t *)realloc(
          model->poligon, model->alloc_poligon * sizeof(poligon_t));
      if (model->poligon == NULL) return ERR_REALLOC;
    }
  }
  for (size_t i = index; i < model->alloc_poligon; i++) {
    model->poligon[i].num_poligon_vertexes = 3;
    model->poligon[i].vertex_array = (long long *)malloc(
        model->poligon->num_poligon_vertexes * sizeof(long long));
    if (model->poligon[i].vertex_array == NULL) return ERR_MALLOC;
  }
  return NO_ERROR;
}

int parser_data(model_3d *model, char *filename) {
  int error = NO_ERROR;
  FILE *file = fopen(filename, "r");
  if (file == NULL) return ERR_NOT_OPEN_FILE;

  error = read_data(file, model);
  fclose(file);
  return error;
}

int read_data(FILE *file, model_3d *model) {
  int error = NO_ERROR;
  char *line = NULL;
  size_t len = 0;
  while (error == NO_ERROR && getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      error = parse_vertex(line + 2, model);
    } else if (line[0] == 'f' && line[1] == ' ') {
      error = parse_facet(line + 2, model);
    }
  }
  free(line);
  return error;
}

int parse_vertex(char *line, model_3d *model) {
  int error = NO_ERROR;
  if (model->vertex_coord.vertex == model->vertex_coord.rows) {
    error = s21_realloc_matrix(&(model->vertex_coord));
  }
  if (error != NO_ERROR) return error;

  char *end = line;
  int i = 0;
  for (float coord = strtof(line, &end); line != end && i < 3;
       coord = strtof(line, &end), i++) {
    model->vertex_coord.matrix[model->vertex_coord.vertex][i] = coord;
    line = end;
    if (errno == ERANGE) {
      return ERR_WRONG_OBJ;
    }
  }
  if (i != 3) {
    return ERR_WRONG_OBJ;
  }
  model->vertex_coord.vertex++;

  return error;
}

int parse_facet(char *line, model_3d *model) {
  int error = NO_ERROR;
  if (model->num_poligon == model->alloc_poligon) {
    model->alloc_poligon *= 2;
    error = init_poligon(model, model->num_poligon);
    if (error != NO_ERROR) return error;
  }

  char *end = NULL;
  int cur_vertex = 0;
  for (long index = strtol(line, &end, 10); line != end;
       index = strtol(line, &end, 10)) {
    if (errno == ERANGE || index == 0 ||
        (index < 0 && (long)model->vertex_coord.vertex - (index + 1) < 0))
      return ERR_WRONG_OBJ;
    if (cur_vertex % 3 == 0) {
      if (index < 0) {
        index = model->vertex_coord.vertex - (index + 1);
      } else {
        index--;
      }
      error = add_index_to_facet(model, (size_t)index, cur_vertex / 3);
      if (error != NO_ERROR) return error;
    }
    line = end;
    while (*line == '/' || *line == ' ') {
      line++;
      cur_vertex++;
    }
  }
  model->num_poligon++;
  return error;
}

int add_index_to_facet(model_3d *model, size_t index, size_t cur_vertex) {
  size_t f = model->num_poligon;
  poligon_t *p = model->poligon;
  if (cur_vertex == p[f].num_poligon_vertexes) {
    p[f].num_poligon_vertexes++;
    p[f].vertex_array = (long long *)realloc(
        p[f].vertex_array, p[f].num_poligon_vertexes * sizeof(long long));
    if (p[f].vertex_array == NULL) return ERR_REALLOC;
  }
  p[f].vertex_array[cur_vertex] = index;
  return NO_ERROR;
}

void free_memory(model_3d *model) {
  for (size_t i = 0; i < model->alloc_poligon; i++) {
    free(model->poligon[i].vertex_array);
  }
  free(model->poligon);
  s21_remove_matrix(&(model->vertex_coord));
}
