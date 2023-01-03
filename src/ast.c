#include "./include/ast.h"
#include <stdlib.h>

/* A very... lightweight version of "inheritance" */
ast_t *init_ast(int type) {
  ast_t *a = (ast_t *)malloc(sizeof(ast_t));
  a->type = type;
  a->car = NULL;
  a->cdr = NULL;
  a->string_value = NULL;
  a->int_value = 0;
  a->float_value = 0;
  a->bool_value = false;
  return a;
}

ast_t *init_ast_string(char *value) {
  ast_t *a = init_ast(AST_STRING);
  a->string_value = value;
  return a;
}

ast_t *init_ast_int(int value) {
  ast_t *a = init_ast(AST_INT);
  a->int_value = value;
  return a;
}

ast_t *init_ast_float(double value) {
  ast_t *a = init_ast(AST_FLOAT);
  a->float_value = value;
  return a;
}

ast_t *init_ast_pair(ast_t *car, ast_t *cdr) {
  ast_t *a = init_ast(AST_PAIR);
  a->car = car;
  a->cdr = cdr;
  return a;
}

ast_t *init_ast_bool(bool value) {
  ast_t *a = init_ast(AST_BOOL);
  a->bool_value = value;
  return a;
}

ast_t *init_ast_symbol(char *value) {
  ast_t *a = init_ast(AST_SYMBOL);
  a->string_value = value;
  return a;
}

ast_t *init_ast_function(ast_t *car, ast_t *cdr) {
  ast_t *a = init_ast(AST_FUNCTION);
  a->car = car;
  a->cdr = cdr;
  return a;
}

ast_t *init_ast_root(ast_t **subnodes, int size) {
  ast_t *a = init_ast(AST_ROOT);
  a->subnodes = subnodes;
  a->root_size = size;
  return a;
}
