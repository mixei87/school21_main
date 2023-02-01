#ifndef S21_ERROR_H_
#define S21_ERROR_H_

typedef enum Error {
  NO_ERROR,
  ERR_NOT_OPEN_FILE,
  ERR_WRONG_OBJ,
  ERR_MALLOC,
  ERR_WRONG_MATRIX,
  ERR_REALLOC,
} Error;

#endif  // S21_ERROR_H_
