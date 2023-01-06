#ifndef VISITOR_H
#define VISITOR_H
#include "./ast.h"
#include "./hash_table.h"
#include "./parser.h"
#include "./stack.h"

typedef struct {
  parser_t *p;
  hash_table_t *eval_table;
  stack_t *stack_frame;
  ast_t *root;
} visitor_t;

void eval_error(visitor_t *v, ast_t *e);

visitor_t *init_visitor(parser_t *p);

bool is_self_evaluating(ast_t *e);

bool is_built_in(ast_t *e);

ast_t *eval_symbol(visitor_t *v, ast_t *e);

ast_t *eval_list(visitor_t *v, ast_t *e);

ast_t *eval_expr(visitor_t *v, ast_t *e);

ast_t *eval(visitor_t *v);
#endif
