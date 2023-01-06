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
  v->stack_frame = init_stack();
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
      strcmp(cmp, "cons") == 0 || strcmp(cmp, "quote") == 0)
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
  /* hash_table_t *lmao = stack_peek(v->stack_frame); */
  if (is_built_in(e))
    return e;
  /* first, it looks in the stack frame for a variable */
  else if (hash_table_exists(stack_peek(v->stack_frame), e->string_value))
    return hash_table_get(stack_peek(v->stack_frame), e->string_value);
  /* Then the variables that have already been evaluated */
  else if (hash_table_exists(v->eval_table, e->string_value))
    return hash_table_get(v->eval_table, e->string_value);

  /* then it goes into the symbol table, evaluates the variable if it finds it
   * and puts it in the list of variables that have already been evaluated */
  else if (hash_table_exists(v->symbol_table, e->string_value)) {
    ast_t *unevaled = hash_table_get(v->symbol_table, e->string_value);
    ast_t *eval = eval_expr(v, unevaled);
    hash_table_add(v->eval_table, e->string_value, eval);
    return eval;
  } else {
    eval_error(v, e);
  }
}

/* Helper function to get the size of an AST linked list; useful for checking
 * parameter number */
int get_list_size(ast_t *root) {
  if (root->type != AST_PAIR)
    return -1;
  int size = 0;
  ast_t *cur = root;
  while (cur->cdr != NULL) {
    size++;
    cur = cur->cdr;
  }
  return size;
}

ast_t *eval_list(visitor_t *v, ast_t *e) {
  ast_t *function = eval_expr(v, e->car);
  ast_t *args = e->cdr;
  int arg_size;
  int cmp = get_list_size(args);
  /* BUILT-IN FUNCTIONS */
  if (function->type == AST_SYMBOL) {
    if (strcmp(function->string_value, "+") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_int(arg1->int_value + arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_float(arg1->int_value + arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_float(arg1->float_value + arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_float(arg1->float_value + arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "-") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_int(arg1->int_value - arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_float(arg1->int_value - arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_float(arg1->float_value - arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_float(arg1->float_value - arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "*") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_int(arg1->int_value * arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_float(arg1->int_value * arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_float(arg1->float_value * arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_float(arg1->float_value * arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "/") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_float(arg1->int_value / arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_float(arg1->int_value / arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_float(arg1->float_value / arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_float(arg1->float_value / arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "%") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_int(arg1->int_value * arg2->int_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "car") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_PAIR) {
        return arg1->car;
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "quote") == 0) {
      if (cmp != 1)
        eval_error(v, e);
      ast_t *arg1 = args->car;
      return arg1;
    } else if (strcmp(function->string_value, "len") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_STRING) {
        return init_ast_int(strlen(arg1->string_value));
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "bool?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_BOOL) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "int?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_INT) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "float?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_FLOAT) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "symbol?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_SYMBOL) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "func?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_FUNCTION) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "pair?") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_PAIR) {
        return init_ast_bool(true);
      } else
        return init_ast_bool(false);
    } else if (strcmp(function->string_value, "atoi") == 0) {
      if (cmp != 1)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);

      if (arg1->type == AST_STRING) {
        int a = atoi(arg1->string_value);
        return init_ast_int(a);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "if") == 0) {
      if (cmp != 3)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);
      ast_t *arg3 = eval_expr(v, args->cdr->cdr->car);
      if (arg1->type == AST_BOOL) {
        if (arg1->bool_value)
          return arg2;
        else
          return arg3;
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "<") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->int_value < arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_bool(arg1->int_value < arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->float_value < arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_bool(arg1->float_value < arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "=") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->int_value == arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_bool(arg1->int_value == arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->float_value == arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_bool(arg1->float_value == arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, ">") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->int_value > arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_bool(arg1->int_value > arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->float_value > arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_bool(arg1->float_value > arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, ">=") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->int_value >= arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_bool(arg1->int_value >= arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->float_value >= arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_bool(arg1->float_value >= arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "<=") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);

      if ((arg1->type == AST_INT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->int_value <= arg2->int_value);
      } else if ((arg1->type == AST_INT) && (arg2->type == AST_FLOAT)) {
        return init_ast_bool(arg1->int_value <= arg2->float_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type == AST_INT)) {
        return init_ast_bool(arg1->float_value <= arg2->int_value);
      } else if ((arg1->type == AST_FLOAT) && (arg2->type) == AST_FLOAT) {
        return init_ast_bool(arg1->float_value <= arg2->float_value);
      } else
        eval_error(v, e);
    } else if (strcmp(function->string_value, "cons") == 0) {
      if (cmp != 2)
        eval_error(v, e);

      ast_t *arg1 = eval_expr(v, args->car);
      ast_t *arg2 = eval_expr(v, args->cdr->car);
      ast_t *ret = init_ast_pair(arg1, arg2);
      return ret;
    }
  }

  /* printf("debug 2\n"); */
  /* NON BUILT-INS */
  /* Checking that the parameters are actually valid */
  if (function->type != AST_FUNCTION)
    eval_error(v, e->car);

  arg_size = get_list_size(function->car);

  /* printf("%d\n", arg_size); */
  if (arg_size != cmp)
    eval_error(v, e->car);
  hash_table_t *stack_frame = init_hash_table(100);

  ast_t *cur_arg_name = function->car;
  ast_t *cur_arg = args;
  char *name;
  ast_t *evaled_arg;
  while (cur_arg->cdr != NULL && cur_arg_name->cdr != NULL) {
    name = cur_arg_name->car->string_value;
    evaled_arg = eval_expr(v, cur_arg->car);
    /* printf("%f\n", evaled_arg->float_value); */
    hash_table_add(stack_frame, name, evaled_arg);
    cur_arg_name = cur_arg_name->cdr;
    cur_arg = cur_arg->cdr;
  }
  stack_push(v->stack_frame, stack_frame);
  ast_t *res = eval_expr(v, function->cdr);
  stack_frame = stack_pop(v->stack_frame);
  /* hash_table_free(stack_frame); */
  return res;
}

ast_t *eval_expr(visitor_t *v, ast_t *e) {
  /* ast_type_print(e); */
  if (is_self_evaluating(e))
    return e;
  else if (e->type == AST_PAIR && is_proper_list(e))
    return eval_list(v, e);
  else if (e->type == AST_SYMBOL)
    return eval_symbol(v, e);
  else {
    eval_error(v, e);
    return NULL;
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

void eval_error(visitor_t *v, ast_t *e) {
  printf("ERROR: something went wrong with the visitor.\n");
  exit(1);
}
