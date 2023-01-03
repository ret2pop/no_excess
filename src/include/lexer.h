#ifndef LEXER_H
#define LEXER_H

#include "./token.h"

#include <stdbool.h>

typedef struct {
  char *source;
  int i;
  char c;

  int row;
  int col;
  bool finished;
} lexer_t;

void lexer_reset(lexer_t *lexer, char *source);

lexer_t *init_lexer(char *source);

void lexer_move(lexer_t *lexer);

void lexer_skip_whitespace(lexer_t *lexer);

void lexer_skip_comment(lexer_t *lexer);

token_t *lexer_collect_bool(lexer_t *lexer);

token_t *lexer_collect_id(lexer_t *lexer);

token_t *lexer_collect_num(lexer_t *lexer);

token_t *lexer_collect_string(lexer_t *lexer);

token_t *lexer_collect_next(lexer_t *lexer);

void lexer_error(lexer_t *lexer);
#endif
