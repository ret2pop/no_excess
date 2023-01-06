#ifndef AST_H
#define AST_H
#include <stdbool.h>

typedef struct AST_STRUCT {
  enum {
    /* root node */
    AST_ROOT,
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
  /* we need to know the amount of expressions in the root node */
  int root_size;
  struct AST_STRUCT **subnodes;

  /* For functions, the car will be a list of variables, and the cdr will be the
   * expression. */
  int argument_number; /* number of arguments that function accepts. Used for
                          speeding up stuff. */
  struct AST_STRUCT *car;
  struct AST_STRUCT *cdr;

  char *string_value; /* Also is symbol value */
  int int_value;
  double float_value;
  bool bool_value;
} ast_t;

ast_t *init_ast(int type);

ast_t *init_ast_string(char *value);

ast_t *init_ast_int(int value);

ast_t *init_ast_float(double value);

ast_t *init_ast_pair(ast_t *car, ast_t *cdr);

ast_t *init_ast_bool(bool value);

ast_t *init_ast_symbol(char *value);

ast_t *init_ast_function(ast_t *car, ast_t *cdr);

ast_t *init_ast_root(ast_t **subnodes, int size);

void ast_type_print(ast_t *e);

bool is_proper_list(ast_t *e);
#endif
