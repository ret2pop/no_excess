#include "./include/parser.h"
#include "./include/hash_table.h"
#include "./include/lexer.h"
#include "./include/macros.h"
#include "./include/token.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

parser_t *init_parser(lexer_t *lexer) {
  parser_t *p = (parser_t *)malloc(sizeof(parser_t));
  if (p == NULL)
    die("malloc on parser");

  p->i = 0;
  p->tokens = malloc(sizeof(token_t *));
  p->symbol_table = init_hash_table();
  p->finished = false;

  if (p->tokens == NULL)
    die("malloc on p->tokens");

  int size = 1;
  token_t *t = lexer_collect_next(lexer);
  p->tokens[size - 1] = t;

  while (true) {
    t = lexer_collect_next(lexer);
    if (t == NULL)
      break;
    size++;
    p->tokens = realloc(p->tokens, size * sizeof(token_t *));
    p->tokens[size - 1] = t;
  }

  p->size = size;
  return p;
}

void parser_move(parser_t *parser) {
  if (parser->i != parser->size - 1)
    parser->i++;
  else
    parser->finished = true;
}

void parser_eat(parser_t *parser, token_t *token) { parser_move(parser); }

ast_t *parse_bool(parser_t *parser) {
  token_t *t = parser->tokens[parser->i];
  parser_move(parser);
  if (strcmp("T", t->value) == 0) {
    return init_ast_bool(true);
  } else
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

ast_t *parse_function(parser_t *parser) {
  parser_move(parser);
  parser_eat(parser, init_token(TOKEN_LPAREN, "(", 0, 0));
  ast_t *car = parse_list(parser); /* gets list of symbols; does not check that
                                      they are symbols :skull: */
  ast_t *cdr =
      parse_expr(parser); /* a function can contain a single expression */
  return init_ast_function(car, cdr);
}

ast_t *parse_list(parser_t *parser) {
  parser_move(parser);
  token_t *cur = parser->tokens[parser->i];
  bool first_entry = true;
  while (cur->type != TOKEN_RPAREN) {
    if (cur->type == TOKEN_ID) {
      if (strcmp(cur->value, "lambda") == 0 && first_entry)
        return parse_function(parser);
      else if (strcmp(cur->value, "bind") == 0 && first_entry)
        return parse_bind(parser);
    } else if (cur->type == TOKEN_LPAREN)
      parse_list(parser);
    first_entry = false;
  }
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
    parser_error(parser);
  }
  return NULL;
}
