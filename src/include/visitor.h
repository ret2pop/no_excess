#ifndef VISITOR_H
#define VISITOR_H
#include "./ast.h"
#include "./hash_table.h"
#include "./stack.h"

typedef struct {
  hash_table_t *symbol_table;
  stack_t *stack_frame;
  ast_t *root;
} visitor_t;

void eval_error(visitor_t *v);

visitor_t *init_visitor(ast_t *root);

bool is_self_evaluating(ast_t *e);

ast_t *eval_symbol(visitor_t *v, ast_t *e);

ast_t *eval_list(visitor_t *v, ast_t *e);

ast_t *eval_expr(visitor_t *v, ast_t *e);

ast_t *eval(visitor_t *v);
#endif
