#include "./include/print.h"
#include "./include/ast.h"
#include <stdio.h>

void print_string(ast_t *str) { printf("%s", str->string_value); }

void print_int(ast_t *i) { printf("%d", i->int_value); }

void print_bool(ast_t *b) {
  if (b->bool_value)
    printf("T");
  else
    printf("F");
}

void print_float(ast_t *f) { printf("%Lf", f->float_value); }

void print_symbol(ast_t *s) { printf(":%s", s->string_value); }

void print_func(ast_t *f) { print_pair(f->cdr); }

void print_pair(ast_t *p) {
  printf("(");
  ast_t *cur = p;
  while (cur != NULL && (cur->cdr != NULL && cur->car != NULL)) {
    print(cur->car);
    if (cur->cdr->cdr != NULL) {
      printf(" ");
    } else
      printf(")");
    cur = cur->cdr;
  }
}

void print(ast_t *res) {
  switch (res->type) {
  case AST_STRING:
    print_string(res);
    break;
  case AST_INT:
    print_int(res);
    break;
  case AST_BOOL:
    print_bool(res);
    break;
  case AST_FLOAT:
    print_float(res);
    break;
  case AST_FUNCTION:
    print_func(res);
    break;
  case AST_SYMBOL:
    print_symbol(res);
    break;
  case AST_PAIR:
    print_pair(res);
    break;
  case AST_ROOT:
    printf("Yeah, let's not do that\n");
    break;
  }
}

void print_root(ast_t *root) {
  for (int i = 0; i < root->root_size; i++)
    print(root->subnodes[i]);
}
