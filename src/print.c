#include "./include/print.h"
#include "./include/ast.h"
#include <stdio.h>

void print_string(ast_t *str) { printf("%s\n", str->string_value); }

void print_int(ast_t *i) { printf("%d\n", i->int_value); }

void print_bool(ast_t *b) {
  if (b->bool_value)
    printf("T\n");
  else
    printf("F\n");
}

void print_float(ast_t *f) { printf("%f\n", f->float_value); }

void print_symbol(ast_t *s) { printf("S: %s\n", s->string_value); }

void print_func(ast_t *f) { print_pair(f->cdr); }

void print_pair(ast_t *p) {
  printf("(");
  ast_t *cur = p;
  bool is_last = false;

  while (cur != NULL && (cur->cdr != NULL && cur->cdr->car != NULL)) {
    if (cur->cdr == NULL) {
      is_last = true;
    }
    switch (cur->car->type) {
    case AST_STRING:
      printf("%s", cur->car->string_value);
      break;
    case AST_INT:
      printf("%d", cur->car->int_value);
      break;
    case AST_FLOAT:
      printf("%d", cur->car->int_value);
      break;
    case AST_BOOL:
      printf("%d", cur->car->int_value);
      break;
    case AST_FUNCTION:
      break;
    case AST_SYMBOL:
      printf("%d", cur->car->int_value);
      break;
    case AST_PAIR:
      printf("%d", cur->car->int_value);
      break;
    case AST_ROOT:
      break;
    }
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
