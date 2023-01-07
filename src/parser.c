#include "./include/parser.h"
#include "./include/hash_table.h"
#include "./include/lexer.h"
#include "./include/macros.h"
#include "./include/token.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

parser_t *init_parser_copy_hash(lexer_t *lexer, hash_table_t *h) {
  parser_t *p = (parser_t *)malloc(sizeof(parser_t));
  if (p == NULL)
    die("malloc on parser");

  p->i = 0;
  p->tokens = malloc(sizeof(token_t *));
  p->symbol_table = h;
  p->finished = false;
  if (p->tokens == NULL)
    die("malloc on p->tokens");

  int size = 1;
  token_t *t = lexer_collect_next(lexer);
  p->tokens[size - 1] = t;

  while (true) {
    t = lexer_collect_next(lexer);
    size++;
    p->tokens = realloc(p->tokens, size * sizeof(token_t *));
    p->tokens[size - 1] = t;
    if (t == NULL)
      break;
  }
  p->size = size;
  return p;
}
parser_t *init_parser(lexer_t *lexer) {
  return init_parser_copy_hash(lexer, init_hash_table(100));
}
/* parser_t *init_parser(lexer_t *lexer) { */
/*   parser_t *p = (parser_t *)malloc(sizeof(parser_t)); */
/*   if (p == NULL) */
/*     die("malloc on parser"); */

/*   p->i = 0; */
/*   p->tokens = malloc(sizeof(token_t *)); */
/*   p->symbol_table = init_hash_table(100); */
/*   p->finished = false; */
/*   if (p->tokens == NULL) */
/*     die("malloc on p->tokens"); */

/*   int size = 1; */
/*   token_t *t = lexer_collect_next(lexer); */
/*   p->tokens[size - 1] = t; */

/*   while (true) { */
/*     t = lexer_collect_next(lexer); */
/*     size++; */
/*     p->tokens = realloc(p->tokens, size * sizeof(token_t *)); */
/*     p->tokens[size - 1] = t; */
/*     if (t == NULL) */
/*       break; */
/*   } */
/*   p->size = size; */
/*   return p; */
/* } */

void parser_move(parser_t *parser) {
  if (parser->i != parser->size - 1)
    parser->i++;
  else
    parser->finished = true;
}

void parser_eat(parser_t *parser, int type) {
  parser_move(parser);
  if (parser->tokens[parser->i] == NULL)
    printf("weird.\n");
  if (parser->tokens[parser->i]->type != type)
    parser_error(parser);
}

ast_t *parse_bool(parser_t *parser) {
  token_t *t = parser->tokens[parser->i];
  parser_move(parser);
  if (strcmp("T", t->value) == 0)
    return init_ast_bool(true);
  else
    return init_ast_bool(false);
}

static char *escape_string(char *str) { return str; }

ast_t *parse_string(parser_t *parser) {
  char *str = parser->tokens[parser->i]->value;
  parser_move(parser);
  return init_ast_string(escape_string(str));
}

ast_t *parse_int(parser_t *parser) {
  int ret = atoi(parser->tokens[parser->i]->value);
  parser_move(parser);
  return init_ast_int(ret);
}

ast_t *parse_float(parser_t *parser) {
  double ret = atof(parser->tokens[parser->i]->value);
  parser_move(parser);
  return init_ast_float(ret);
}

ast_t *parse_symbol(parser_t *parser) {
  char *str = parser->tokens[parser->i]->value;
  parser_move(parser);
  return init_ast_symbol(str);
}

ast_t *parse_function_args(parser_t *parser) {
  ast_t *car;
  ast_t *head = init_ast_pair(NULL, NULL);
  ast_t *cur = head;
  parser_move(parser);

  token_t *current_token = parser->tokens[parser->i];
  while (current_token->type != TOKEN_RPAREN) {
    if (current_token->type != TOKEN_ID)
      parser_error(parser);

    car = parse_symbol(parser);

    cur->car = car;
    cur->cdr = init_ast_pair(NULL, NULL);
    cur = cur->cdr;

    current_token = parser->tokens[parser->i];
  }

  parser_move(parser);
  return head;
}

ast_t *parse_function(parser_t *parser) {
  parser_eat(parser, TOKEN_LPAREN);
  ast_t *car = parse_function_args(parser);
  ast_t *cdr =
      parse_expr(parser); /* a function can contain a single expression */
  if (cdr == NULL)
    parser_error(parser);
  if (parser->tokens[parser->i]->type != TOKEN_RPAREN)
    parser_error(parser);
  parser_move(parser);
  return init_ast_function(car, cdr);
}

void parse_bind(parser_t *parser) {
  parser_move(parser);
  if (parser->tokens[parser->i]->type != TOKEN_ID)
    parser_error(parser);
  token_t *t = parser->tokens[parser->i];
  char *name = t->value;
  parser_move(parser);
  ast_t *expr = parse_expr(parser); /* unevaluated expr will be evaluated when
                                       hash table transfers to visitor JIT */
  if (expr == NULL)
    parser_error(parser);
  if (expr->type == AST_ROOT)
    parser_error(parser);
  hash_table_add(parser->symbol_table, name, expr);
  if (parser->tokens[parser->i]->type != TOKEN_RPAREN)
    parser_error(parser);

  parser_move(parser);
}

ast_t *parse_include(parser_t *parser) {
  parser_move(parser);
  if (parser->tokens[parser->i]->type != TOKEN_STRING)
    parser_error(parser);
  char *filename = parser->tokens[parser->i]->value;
  char *buffer = 0;
  long length;
  FILE *f = fopen(filename, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  } else {
    parser_error(parser);
  }
  if (buffer) {
    lexer_t *lexer = init_lexer(buffer);
    parser_t *p = init_parser_copy_hash(lexer, parser->symbol_table);
    ast_t *root = parse_all(p);
    parser_move(parser);
    if (parser->tokens[parser->i]->type != TOKEN_RPAREN)
      parser_error(parser);
    parser_move(parser);
    return root;
  } else {
    parser_error(parser);
  }
  return NULL;
}

ast_t *parse_list(parser_t *parser) {
  ast_t *car;
  ast_t *head = init_ast_pair(NULL, NULL);
  ast_t *cur = head;
  parser_move(parser);

  bool first_entry = true;
  token_t *current_token = parser->tokens[parser->i];
  while (current_token->type != TOKEN_RPAREN) {
    if (current_token->type == TOKEN_ID) {
      if (strcmp(current_token->value, "lambda") == 0 && first_entry) {
        return parse_function(parser);
      } else if (strcmp(current_token->value, "bind") == 0 && first_entry) {
        parse_bind(parser);
        return NULL;
      } else if (strcmp(current_token->value, "include") == 0 && first_entry) {
        return parse_include(parser);
      } else {
        car = parse_symbol(parser);
      }
    } else {
      car = parse_expr(parser);
      if (car == NULL)
        parser_error(parser);
      if (car->type == AST_ROOT)
        parser_error(parser);
    }
    cur->car = car;
    cur->cdr = init_ast_pair(NULL, NULL);
    cur = cur->cdr;
    first_entry = false;
    current_token = parser->tokens[parser->i];
  }
  parser_move(parser);
  return head;
}

ast_t *parse_quote(parser_t *parser) {
  parser_move(parser);
  ast_t *car = init_ast_symbol("quote");
  ast_t *expr = parse_expr(parser);
  if (expr == NULL)
    parser_error(parser);
  if (expr->type == AST_ROOT)
    parser_error(parser);
  ast_t *ret = init_ast_pair(
      car, init_ast_pair(
               expr, init_ast_pair(NULL, NULL))); /* Converts ' to `quote` */
  return ret;
}

ast_t *parse_expr(parser_t *parser) {
  token_t *t = parser->tokens[parser->i];
  if (t->type == TOKEN_STRING)
    return parse_string(parser);
  else if (t->type == TOKEN_INT)
    return parse_int(parser);
  else if (t->type == TOKEN_FLOAT)
    return parse_float(parser);
  else if (t->type == TOKEN_BOOL)
    return parse_bool(parser);
  else if (t->type == TOKEN_LPAREN)
    return parse_list(parser);
  else if (t->type == TOKEN_QUOTE)
    return parse_quote(parser);
  else if (t->type == TOKEN_ID)
    return parse_symbol(parser);
  else if (t->type == TOKEN_LPAREN)
    return parse_list(parser);
  else {
    printf("%d", t->type);
    parser_error(parser);
  }
  return NULL;
}

ast_t *parse_all(parser_t *parser) {
  token_t *t = parser->tokens[parser->i];
  ast_t **asts = malloc(sizeof(ast_t *));
  ast_t *cur;
  int i = 0;
  while (t != NULL) {
    cur = parse_expr(parser);
    if (cur == NULL) {
      t = parser->tokens[parser->i];
      continue;
    } else if (cur->type == AST_ROOT) {
      for (int j = 0; j < cur->root_size; j++) {
        i++;
        asts = realloc(asts, i * sizeof(ast_t *));
        asts[i - 1] = cur->subnodes[j];
      }
      t = parser->tokens[parser->i];
      continue;
    }
    i++;
    asts = realloc(asts, i * sizeof(ast_t *));
    asts[i - 1] = cur;
    t = parser->tokens[parser->i];
  }
  return init_ast_root(asts, i);
}

void parser_error(parser_t *parser) {
  printf("PARSER ERROR: something went wrong.\n");
  exit(1);
}
