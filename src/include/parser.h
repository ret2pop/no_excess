#ifndef PARSER_H
#define PARSER_H
#include "./ast.h"
#include "./hash_table.h"
#include "./lexer.h"
#include "./token.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  token_t **tokens;
  hash_table_t *symbol_table;
  int i;
  int size;
  bool finished;
  bool is_global;
} parser_t;

parser_t *init_parser(lexer_t *lexer);

void parser_error(parser_t *parser);

void parser_move(parser_t *parser);

void parser_eat(parser_t *parser, int type);

ast_t *parse_string(parser_t *parser);

ast_t *parse_int(parser_t *parser);

ast_t *parse_float(parser_t *parser);

ast_t *parse_bool(parser_t *parser);

ast_t *parse_list(parser_t *parser);

ast_t *parse_quote(parser_t *parser);

ast_t *parse_symbol(parser_t *parser);

ast_t *parse_function(parser_t *parser);

void parse_bind(parser_t *parser);

ast_t *parse_expr(parser_t *parser);

ast_t *parse_all(parser_t *parser);

void parser_free(parser_t *parser);
#endif
