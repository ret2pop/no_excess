#ifndef AST_H
#define AST_H
#include <stdbool.h>

typedef struct AST_STRUCT {
  enum {
    /* complex types */
    AST_PAIR,
    AST_SYMBOL,
    /* self evaluating types */
    AST_STRING,
    AST_BOOL,
    AST_INT,
    AST_FLOAT,
    AST_FUNCTION,
  } type;

  /* For functions, the car will be a list of variables, and the cdr will be the
   * expression */
  struct AST_STRUCT *car;
  struct AST_STRUCT *cdr;

  char *string_value;
  int int_value;
  double float_value;
  bool bool_value;
} ast_t;

ast_t *init_ast_string(char *value);

ast_t *init_ast_int(int value);

ast_t *init_ast_float(double value);

ast_t *init_ast_pair(ast_t *car, ast_t *cdr);

ast_t *init_ast_bool(bool value);

ast_t *init_ast_symbol(char *value);

ast_t *init_ast_function(ast_t *car, ast_t *cdr);
#endif
