#ifndef SRC_REVERSE_POLISH_NOTATION_H_
#define SRC_REVERSE_POLISH_NOTATION_H_
#include <math.h>
#include <stdlib.h>
#define MAX_LENGTH_OPERATOR 4
#define COUNT_LEXEM 32

// don't touch order!
// char str_lexem[86] =
//     "( ) * + - / ^ acos asin atan cos ln log mod "
//     "sin sqrt tan x + - . 0 1 2 3 4 5 6 7 8 9 E";
typedef enum Lexem {
  LEX_lbracket,
  LEX_rbracket,
  LEX_asterisk,
  LEX_plus,
  LEX_minus,
  LEX_div,
  LEX_pow,
  LEX_acos,
  LEX_asin,
  LEX_atan,
  LEX_cos,
  LEX_ln,
  LEX_log,
  LEX_mod,
  LEX_sin,
  LEX_sqrt,
  LEX_tan,
  LEX_x,
  LEX_unary_plus,
  LEX_unary_minus,
  LEX_num,
  LEX_noth,
} Lexem;

typedef enum Associate { LEFT, RIGHT } Associate;
typedef double (*Func)(double);
typedef double (*Operation)(double, double);

typedef struct StackIn {
  Lexem lexem;
  int priority;
  Associate associate;
  int num_prev_operations;
  Func func;
  Operation operation;
  struct StackIn *next;
} StackIn;

typedef struct StackOut {
  Lexem lexem;
  double x;
  int num_prev_operations;
  Func func;
  Operation operation;
  struct StackOut *next;
  struct StackOut *prev;
} StackOut;

typedef struct ArrStack {
  StackIn *input, *operators, *functions;
  StackOut *output;
  char **arr_lexem;
} ArrStack;

void initFuncAndOperators(ArrStack **stack);
void pushInStack(StackIn **src, StackIn *dst);
void pushIn(StackIn **head, Lexem lexem, int priority, Associate associate,
            int num_prev_operations, Func func, Operation operation);
// delete only from head of stack
void deleteInNode(StackIn **head);
void deleteOutPrevNode(StackOut *node);
void moveInputToOutput(StackIn **input_head, StackOut **output_head);
void pushOutStack(StackOut **new_stack, StackOut *old_stack);
void pushOut(StackOut **head, Lexem lexem, double x, int num_prev_operations,
             Func func, Operation operation);
void copyStack(StackOut **new_stack, StackOut *old_stack);
// 0 - good, 1 - error
// RPN: ReversePolishNotation
int calculateRPN(char *str, StackOut **output, double x, double *y);
int calculateStack(StackOut *output, double x_input, double *res);
int createStackRPN(char *str, ArrStack *stacks);
char **initArrLexem();
void removeArr(char **arr);
// 0 - error, 1 - good, 2 - need asterisk
char **initMatrixAdjacency();
void handlerRPN(StackOut *ptr, double x, double *y);
int parseString(char **str, char **arr_lexem, Lexem *lexem, double *number);
int calculateAlgorithmDijkstra(ArrStack *stacks, Lexem lexem, double number);
int findStackIn(StackIn *head, Lexem lexem, StackIn **ptr_lexem);
void removeStackIn(StackIn *head);
void removeStackOut(StackOut *head);
void freeOtherStacks(ArrStack **stacks);
double unaryPlus(double x);
double unaryMinus(double x);
double Mul(double x, double y);
double Div(double x, double y);
double Add(double x, double y);
double Sub(double x, double y);
#endif  // SRC_REVERSE_POLISH_NOTATION_H_
