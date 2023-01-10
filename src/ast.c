#include "./include/ast.h"
#include <stdio.h>
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

ast_t *init_ast_float(long double value) {
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

void ast_type_print(ast_t *e) {
  if (e->type == AST_FUNCTION) {
    printf("Function\n");
  } else if (e->type == AST_INT) {
    printf("Integer\n");
  } else if (e->type == AST_FLOAT) {
    printf("Float\n");
  } else if (e->type == AST_BOOL) {
    printf("Bool\n");
  } else if (e->type == AST_SYMBOL) {
    printf("Symbol\n");
  } else if (e->type == AST_PAIR) {
    printf("Pair\n");
  } else if (e->type == AST_STRING) {
    printf("String\n");
  } else if (e->type == AST_ROOT) {
    printf("Root Node\n");
  }
}

bool is_proper_list(ast_t *e) {
  ast_t *cur = e;
  while (cur->cdr != NULL) {
    cur = cur->cdr;
  }
  if (cur->type == AST_PAIR && cur->car == NULL)
    return true;
  return false;
}

void ast_free(ast_t *e) {
  if (e->type == AST_PAIR) {
    ast_t *cur = e;
    ast_t *tmp;
    while (cur != NULL) {
      tmp = cur;
      cur = cur->cdr;
      free(tmp);
    }
  } else if (e->type == AST_ROOT) {
    for (int i = 0; i < e->root_size; i++) {
      ast_free(e->subnodes[i]);
    }
    free(e);
  } else {
    free(e);
  }
}
