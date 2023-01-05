#include "./include/visitor.h"
#include "./include/hash_table.h"
#include "./include/macros.h"
#include "./include/parser.h"
#include "./include/stack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

visitor_t *init_visitor(parser_t *p) {
  visitor_t *v = (visitor_t *)malloc(sizeof(visitor_t));
  if (v == NULL)
    die("malloc on visitor");
  v->stack_frame = init_stack(512);
  v->symbol_table = p->symbol_table;
  v->eval_table = init_hash_table(1000);
  v->root = parse_all(p);
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

bool is_built_in(ast_t *e) {
  char *cmp = e->string_value;
  /* Basic mathematics */
  if (strcmp(cmp, "*") == 0 || strcmp(cmp, "+") == 0 || strcmp(cmp, "-") == 0 ||
      strcmp(cmp, "/") == 0 || strcmp(cmp, "%") == 0)
    return true;

  /* Some string and list operations */
  if (strcmp(cmp, "concat") == 0 || strcmp(cmp, "len") == 0 ||
      strcmp(cmp, "car") == 0 || strcmp(cmp, "cdr") == 0 ||
      strcmp(cmp, "cons") == 0)
    return true;

  /* Comparison functions */
  if (strcmp(cmp, "<") == 0 || strcmp(cmp, ">") == 0 || strcmp(cmp, "=") == 0 ||
      strcmp(cmp, "<=") == 0 || strcmp(cmp, ">=") == 0 ||
      strcmp(cmp, "eq") == 0)
    return true;

  /* Type-checking */
  if (strcmp(cmp, "bool?") == 0 || strcmp(cmp, "int?") == 0 ||
      strcmp(cmp, "symbol?") == 0 || strcmp(cmp, "float?") == 0 ||
      strcmp(cmp, "string?") == 0 || strcmp(cmp, "pair?") == 0 ||
      strcmp(cmp, "func?") == 0)
    return true;

  /* Type conversions */
  if (strcmp(cmp, "atoi") == 0 || strcmp(cmp, "itof") == 0 ||
      strcmp(cmp, "ftoi") == 0 || strcmp(cmp, "itoa") == 0 ||
      strcmp(cmp, "atof") == 0 || strcmp(cmp, "ftoa") == 0)
    return true;

  return false;
}
/* Special symbols: car, cdr, quote, *, /, +, -, %, inc, dec, >, <, >=, <=, /=,
 * =, equal (for strings), input */
ast_t *eval_symbol(visitor_t *v, ast_t *e) {
  if (is_built_in(e))
    return e;

  else if (hash_table_exists(v->eval_table, e->string_value))
    return hash_table_get(v->eval_table, e->string_value);
  else if (hash_table_exists(stack_peek(v->stack_frame), e->string_value))
    return hash_table_get(stack_peek(v->stack_frame), e->string_value);

  else if (hash_table_exists(v->symbol_table, e->string_value)) {
    ast_t *unevaled = hash_table_get(v->symbol_table, e->string_value);
    ast_t *eval = eval_expr(v, unevaled);
    hash_table_add(v->eval_table, e->string_value, eval);
    return eval;
  } else
    return e;
}

ast_t *eval_list(visitor_t *v, ast_t *e) {}

ast_t *eval_expr(visitor_t *v, ast_t *e) {
  if (is_self_evaluating(e))
    return e;
  else if (e->type == AST_PAIR)
    return eval_list(v, e);
  else if (e->type == AST_SYMBOL)
    return eval_symbol(v, e);
  else {
    eval_error(v, e);
  }
}

ast_t *eval(visitor_t *v) {
  ast_t *cur;
  ast_t *root;
  ast_t **eval_nodes = malloc(sizeof(ast_t *));
  for (int i = 0; i < v->root->root_size; i++) {
    cur = eval_expr(v, v->root->subnodes[i]);
    eval_nodes = realloc(eval_nodes, (i + 1) * sizeof(ast_t *));
    eval_nodes[i] = cur;
  }
  root = init_ast_root(eval_nodes, v->root->root_size);
  return root;
}

void eval_error(visitor_t *v, ast_t *e) { exit(1); }
