#include "./include/visitor.h"
#include "./include/hash_table.h"
#include "./include/macros.h"
#include "./include/stack.h"
#include <stdlib.h>

visitor_t *init_visitor(ast_t *root) {
  visitor_t *v = (visitor_t *)malloc(sizeof(visitor_t));
  if (v == NULL)
    die("malloc on visitor");
  v->stack_frame = init_stack(512);
  v->symbol_table = init_hash_table(10000);
  v->root = root;
  return v;
}

bool is_self_evaluating(ast_t *e) {
  if (e->type == AST_STRING || e->type == AST_INT || e->type == AST_FLOAT ||
      e->type == AST_BOOL || e->type == AST_FUNCTION)
    return true;

  if (e->type == AST_PAIR && e->car == NULL && e->cdr == NULL)
    return true;
  return false;
}
