#include "reverse_polish_notation.h"

void initFuncAndOperators(ArrStack **stack) {
  pushIn(&((*stack)->functions), LEX_lbracket, 0, RIGHT, 0, NULL, NULL);
  pushIn(&((*stack)->functions), LEX_cos, 0, RIGHT, 1, &cos, NULL);
  pushIn(&((*stack)->functions), LEX_sin, 0, RIGHT, 1, &sin, NULL);
  pushIn(&((*stack)->functions), LEX_tan, 0, RIGHT, 1, &tan, NULL);
  pushIn(&((*stack)->functions), LEX_acos, 0, RIGHT, 1, &acos, NULL);
  pushIn(&((*stack)->functions), LEX_asin, 0, RIGHT, 1, &asin, NULL);
  pushIn(&((*stack)->functions), LEX_atan, 0, RIGHT, 1, &atan, NULL);
  pushIn(&((*stack)->functions), LEX_sqrt, 0, RIGHT, 1, &sqrt, NULL);
  pushIn(&((*stack)->functions), LEX_ln, 0, RIGHT, 1, &log, NULL);
  pushIn(&((*stack)->functions), LEX_log, 0, RIGHT, 1, &log10, NULL);
  pushIn(&((*stack)->operators), LEX_unary_plus, 0, RIGHT, 1, &unaryPlus, NULL);
  pushIn(&((*stack)->operators), LEX_unary_minus, 0, RIGHT, 1, &unaryMinus,
         NULL);
  pushIn(&((*stack)->operators), LEX_pow, 1, RIGHT, 2, NULL, &pow);
  pushIn(&((*stack)->operators), LEX_asterisk, 2, LEFT, 2, NULL, &Mul);
  pushIn(&((*stack)->operators), LEX_div, 2, LEFT, 2, NULL, &Div);
  pushIn(&((*stack)->operators), LEX_mod, 2, LEFT, 2, NULL, &fmod);
  pushIn(&((*stack)->operators), LEX_plus, 3, LEFT, 2, NULL, &Add);
  pushIn(&((*stack)->operators), LEX_minus, 3, LEFT, 2, NULL, &Sub);
}

void pushInStack(StackIn **src, StackIn *dst) {
  pushIn(src, dst->lexem, dst->priority, dst->associate,
         dst->num_prev_operations, dst->func, dst->operation);
}

void pushIn(StackIn **head, Lexem lexem, int priority, Associate associate,
            int num_prev_operations, Func func, Operation operation) {
  StackIn *new_node = (StackIn *)malloc(sizeof(StackIn));
  new_node->lexem = lexem;
  new_node->priority = priority;
  new_node->associate = associate;
  new_node->num_prev_operations = num_prev_operations;
  new_node->func = func;
  new_node->operation = operation;
  new_node->next = *head;
  *head = new_node;
}

void deleteInNode(StackIn **head) {
  if (*head == NULL) return;
  StackIn *tmp = *head;
  *head = (*head)->next;
  free(tmp);
}

void deleteOutPrevNode(StackOut *node) {
  if (node == NULL) return;
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  node->prev->next = node->next;
  free(node);
}

void moveInputToOutput(StackIn **input_head, StackOut **output_head) {
  if (*input_head == NULL) return;
  pushOut(output_head, (*input_head)->lexem, 0.0,
          (*input_head)->num_prev_operations, (*input_head)->func,
          (*input_head)->operation);
  StackIn *tmp = *input_head;
  *input_head = (*input_head)->next;
  free(tmp);
}

void pushOutStack(StackOut **new_stack, StackOut *old_stack) {
  pushOut(new_stack, old_stack->lexem, old_stack->x,
          old_stack->num_prev_operations, old_stack->func,
          old_stack->operation);
}

void pushOut(StackOut **head, Lexem lexem, double x, int num_prev_operations,
             Func func, Operation operation) {
  StackOut *new_node = (StackOut *)malloc(sizeof(StackOut));
  new_node->x = x;
  new_node->lexem = lexem;
  new_node->num_prev_operations = num_prev_operations;
  if (*head != NULL) {
    (*head)->prev = new_node;
  }
  new_node->func = func;
  new_node->operation = operation;
  new_node->prev = NULL;
  new_node->next = *head;
  *head = new_node;
}

void copyStack(StackOut **new_stack, StackOut *old_stack) {
  if (old_stack == NULL) return;
  while (old_stack->next != NULL) {
    old_stack = old_stack->next;
  }
  while (old_stack != NULL) {
    pushOutStack(new_stack, old_stack);
    old_stack = old_stack->prev;
  }
}

int calculateRPN(char *str, StackOut **output, double x, double *y) {
  int error = 0;
  if (*str != 0) {
    ArrStack stacks = {.input = NULL,
                       .operators = NULL,
                       .functions = NULL,
                       .output = *output,
                       .arr_lexem = NULL};
    if (stacks.output == NULL) {
      stacks.arr_lexem = initArrLexem();
      error = createStackRPN(str, &stacks);
      *output = stacks.output;
      removeArr(stacks.arr_lexem);
    }
    calculateStack(*output, x, y);
  }
  return error;
}

int calculateStack(StackOut *output, double x, double *y) {
  int error = 0;
  StackOut *copy_output = NULL;
  copyStack(&copy_output, output);
  handlerRPN(copy_output, x, y);
  removeStackOut(copy_output);
  return error;
}

int createStackRPN(char *str, ArrStack *stacks) {
  int error = 0;
  Lexem lexem_prev = LEX_noth, lexem_cur = LEX_noth;
  double number = 0;

  initFuncAndOperators(&stacks);
  char **matrix_adjacency = initMatrixAdjacency();

  while (error == 0 && *str != 0) {
    error = parseString(&str, stacks->arr_lexem, &lexem_cur, &number);
    if (error == 0) {
      if (matrix_adjacency[lexem_prev][lexem_cur] == 0) {
        error = 1;
      } else if (matrix_adjacency[lexem_prev][lexem_cur] == 1) {
        error = calculateAlgorithmDijkstra(stacks, lexem_cur, number);
      } else {
        error = calculateAlgorithmDijkstra(stacks, LEX_asterisk, number);
        if (error == 0)
          error = calculateAlgorithmDijkstra(stacks, lexem_cur, number);
      }
      lexem_prev = lexem_cur;
    }
  }
  removeArr(matrix_adjacency);
  if (lexem_cur != LEX_rbracket && lexem_cur != LEX_x && lexem_cur != LEX_num) {
    error = 1;
  }
  while (error == 0 && stacks->input != NULL) {
    if (stacks->input->lexem == LEX_lbracket) {
      error = 1;
    } else {
      moveInputToOutput(&(stacks->input), &(stacks->output));
    }
  }
  removeStackIn(stacks->input);
  freeOtherStacks(&stacks);
  return error;
}

char **initArrLexem() {
  char **arr_lexem = (char **)malloc(COUNT_LEXEM * sizeof(char *));
  for (int i = 0; i < COUNT_LEXEM; i++) {
    arr_lexem[i] = (char *)calloc(MAX_LENGTH_OPERATOR + 1, sizeof(char));
  }
  // lexem with length > 1 - sort by ascending
  char str_lexem[87] =
      "( ) * + - / ^ acos asin atan cos ln log mod "
      "sin sqrt tan x + - . 0 1 2 3 4 5 6 7 8 9 E";
  char *ptr = str_lexem;
  int i = 0;
  while (*ptr != 0) {
    int j = 0;
    while (*ptr != 0 && *ptr != ' ') {
      arr_lexem[i][j] = *ptr;
      j++;
      ptr++;
    }
    i++;
    if (*ptr != 0) ptr++;
  }
  return arr_lexem;
}

void removeArr(char **arr) {
  for (int i = 0; i < COUNT_LEXEM; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  free(arr);
}

char **initMatrixAdjacency() {
  char **matrix_adjacency = (char **)malloc(COUNT_LEXEM * sizeof(char *));
  for (int i = 0; i < COUNT_LEXEM; i++) {
    matrix_adjacency[i] = (char *)calloc(COUNT_LEXEM, sizeof(char));
  }
  // functions may be after any lexem
  for (int i = LEX_lbracket; i <= LEX_noth; i++) {
    for (int j = LEX_acos; j <= LEX_tan; j++) {
      if (i == LEX_rbracket) break;
      if (j == LEX_mod) continue;
      if (i == LEX_x || i == LEX_num)
        matrix_adjacency[i][j] = 2;
      else
        matrix_adjacency[i][j] = 1;
    }
    if (i == LEX_rbracket || i == LEX_x || i == LEX_num)
      matrix_adjacency[i][LEX_lbracket] = 2;
    else
      matrix_adjacency[i][LEX_lbracket] = 1;
  }
  // after "(" || "^" || "*" || "/" || "mod" || "+" || "-" || "functions" ||
  // "u+" || "u-" || LEX_noth may be: "functions" || "x" || "u+" || "u-" ||
  // "num"
  for (int i = LEX_lbracket; i <= LEX_noth; i++) {
    if (i == LEX_rbracket || i == LEX_num || i == LEX_x) continue;
    matrix_adjacency[i][LEX_x] = 1, matrix_adjacency[i][LEX_unary_plus] = 1,
    matrix_adjacency[i][LEX_unary_minus] = 1, matrix_adjacency[i][LEX_num] = 1;
  }
  // after "x" || "num" may be: ")" || "*" || "+" || "-" || "/" || "^"
  for (int i = LEX_rbracket; i <= LEX_pow; i++) {
    matrix_adjacency[LEX_x][i] = 1;
    matrix_adjacency[LEX_num][i] = 1;
  }
  matrix_adjacency[LEX_x][LEX_num] = 2;
  matrix_adjacency[LEX_num][LEX_x] = 2;
  matrix_adjacency[LEX_x][LEX_mod] = 1;
  matrix_adjacency[LEX_num][LEX_mod] = 1;
  // after ")" may be: all lexems except LEX_unary_plus, LEX_unary_minus,
  // LEX_noth
  for (int i = LEX_rbracket; i <= LEX_num; i++) {
    if ((i >= LEX_acos && i <= LEX_log) || (i >= LEX_sin && i <= LEX_x) ||
        i == LEX_num) {
      matrix_adjacency[LEX_rbracket][i] = 2;
    } else if (i == LEX_unary_plus || i == LEX_unary_minus) {
      continue;
    } else {
      matrix_adjacency[LEX_rbracket][i] = 1;
    }
  }
  return matrix_adjacency;
}

void handlerRPN(StackOut *ptr, double x_input, double *x_res) {
  if (ptr == NULL) return;
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  while (ptr != NULL) {
    if (ptr->lexem == LEX_x) {
      ptr->lexem = LEX_num;
      ptr->x = x_input;
    }
    int num_prev_operations = ptr->num_prev_operations;
    if (num_prev_operations != 0 && ptr->next != NULL) {
      ptr->lexem = LEX_num;
      if (num_prev_operations == 1) {
        ptr->x = ptr->func(ptr->next->x);
      } else if (num_prev_operations == 2 && ptr->next->next != NULL) {
        ptr->x = ptr->operation(ptr->next->next->x, ptr->next->x);
        deleteOutPrevNode(ptr->next);
      }
      deleteOutPrevNode(ptr->next);
    }
    if (ptr->prev == NULL) {
      *x_res = ptr->x;
    }
    ptr = ptr->prev;
  }
}

int parseString(char **str, char **arr_lexem, Lexem *lexem, double *number) {
  int error = 0, i = 0;
  char *pos = *str;
  if ((pos[i] >= '0' && pos[i] <= '9') || pos[i] == '.' || pos[i] == 'E' ||
      pos[i] == 'e') {
    // handlerOfNumber
    int num_point = 0, num_E = 0;
    for (; error == 0 && pos[i] != 0 &&
           ((pos[i] >= '0' && pos[i] <= '9') || pos[i] == '.' ||
            pos[i] == 'E' || pos[i] == 'e' || pos[i] == '+' || pos[i] == '-');
         i++) {
      if (pos[i] == '.') {
        // incorrect: "[^\.]*\.[^\.]*\.", ".E", "^\.[^0-9]"
        if (++num_point > 1 || pos[i + 1] == 'E' || pos[i + 1] == 'e' ||
            (i == 0 && !(pos[1] >= '0' && pos[1] <= '9'))) {
          error = 1;
        }
      } else if ((pos[i] == 'E' || pos[i] == 'e') && (i == 0 || ++num_E > 1)) {
        // incorrect: "^E", "[^E]*E[^E]*E"
        error = 1;
      } else if (pos[i] == '+' || pos[i] == '-') {
        if (!((pos[i - 1] == 'E' || pos[i - 1] == 'e') && pos[i + 1] >= '0' &&
              pos[i + 1] <= '9')) {
          break;
        }
      }
    }
    if (error == 0) {
      *number = atof(*str);
      *str = pos + i;
      *lexem = LEX_num;
    }
  } else {
    error = 1;
    for (int j = 0; error == 1 && i < COUNT_LEXEM && pos[j] != 0; i++) {
      for (; error == 1 && j < MAX_LENGTH_OPERATOR &&
             pos[j] == arr_lexem[i][j];) {
        if (arr_lexem[i][++j] == 0) {
          error = 0;
          if ((pos[0] == '-' || pos[0] == '+') && *lexem != LEX_num &&
              *lexem != LEX_rbracket && *lexem != LEX_x) {
            if (pos[0] == '-') {
              *lexem = LEX_unary_minus;
            } else {
              *lexem = LEX_unary_plus;
            }
          } else {
            *lexem = (Lexem)i;
          }
          *str = pos + j;
        }
      }
    }
  }
  return error;
}

int calculateAlgorithmDijkstra(ArrStack *stacks, Lexem lexem, double number) {
  int error = 0;
  if (lexem == LEX_x) {
    pushOut(&stacks->output, LEX_x, 0.0, 0, NULL, NULL);
  } else if (lexem == LEX_num) {
    pushOut(&stacks->output, LEX_num, number, 0, NULL, NULL);
  } else {
    StackIn *ptr_lexem = NULL;
    if (findStackIn(stacks->functions, lexem, &ptr_lexem)) {
      pushInStack(&stacks->input, ptr_lexem);
    } else if (findStackIn(stacks->operators, lexem, &ptr_lexem)) {
      while (stacks->input != NULL && stacks->input->lexem != LEX_lbracket &&
             (stacks->input->priority < ptr_lexem->priority ||
              (stacks->input->priority == ptr_lexem->priority &&
               ptr_lexem->associate == LEFT))) {
        moveInputToOutput(&stacks->input, &stacks->output);
      }
      pushInStack(&stacks->input, ptr_lexem);
    } else if (lexem == LEX_rbracket) {
      error = 1;
      while (
          stacks->input != NULL &&
          (findStackIn(stacks->functions, stacks->input->lexem, &ptr_lexem) ||
           findStackIn(stacks->operators, stacks->input->lexem, &ptr_lexem))) {
        if (ptr_lexem->lexem == LEX_lbracket) {
          error = 0;
          deleteInNode(&stacks->input);
          break;
        }
        moveInputToOutput(&stacks->input, &stacks->output);
      }
    }
  }
  return error;
}

int findStackIn(StackIn *head, Lexem lexem, StackIn **ptr_lexem) {
  int flag_find = 0;
  while (flag_find == 0 && head != NULL) {
    if (head->lexem == lexem) {
      flag_find = 1;
      *ptr_lexem = head;
    }
    head = head->next;
  }
  return flag_find;
}

void removeStackIn(StackIn *head) {
  while (head != NULL) {
    StackIn *tmp = head;
    head = tmp->next;
    free(tmp);
  }
}

void removeStackOut(StackOut *head) {
  while (head != NULL) {
    StackOut *tmp = head;
    head = tmp->next;
    free(tmp);
  }
}

void freeOtherStacks(ArrStack **stacks) {
  removeStackIn((*stacks)->operators);
  removeStackIn((*stacks)->functions);
}

double unaryPlus(double x) { return x; }
double unaryMinus(double x) { return -x; }
double Mul(double x, double y) { return x * y; }
double Div(double x, double y) { return x / y; }
double Add(double x, double y) { return x + y; }
double Sub(double x, double y) { return x - y; }
