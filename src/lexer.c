#include "./include/lexer.h"
#include "./include/macros.h"
#include "./include/token.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lexer_reset(lexer_t *lexer, char *source) {
  lexer->source = source;
  lexer->c = source[0];
  lexer->i = 0;
  lexer->row = 1;
  lexer->col = 1;
  lexer->finished = false;
}

lexer_t *init_lexer(char *source) {
  lexer_t *l = (lexer_t *)malloc(sizeof(lexer_t));
  if (l == NULL)
    die("malloc on lexer");
  lexer_reset(l, source);
  return l;
}

void lexer_move(lexer_t *lexer) {
  if (lexer->c != '\0' && lexer->c != EOF) {
    lexer->i++;
    lexer->c = lexer->source[lexer->i];
    if (lexer->c == '\n') {
      lexer->row++;
      lexer->col = 0;
    }
  } else
    lexer->finished = true;
}

void lexer_ignore_whitespace(lexer_t *lexer) {
  while (isspace(lexer->c)) {
    if (lexer->c == '\0' || lexer->c == EOF)
      return;
    lexer_move(lexer);
  }
}

void lexer_skip_comment(lexer_t *lexer) {
  while (lexer->c != '\n' && lexer->c != '\0')
    lexer_move(lexer);
}

static bool is_valid_id_char(char c) {
  if (c == '(' || c == ')' || isdigit(c) || c == '"' || c == '\'' || c == '#' ||
      c == '.' || isspace(c))
    return false;
  return true;
}

token_t *lexer_collect_bool(lexer_t *lexer) {
  lexer_move(lexer);
  if (lexer->c == 't') {
    lexer_move(lexer);
    return init_token(TOKEN_BOOL, "T", lexer->row, lexer->col);
  } else if (lexer->c == 'f') {
    lexer_move(lexer);
    return init_token(TOKEN_BOOL, "F", lexer->row, lexer->col);
  } else
    return NULL;
}

token_t *lexer_collect_id(lexer_t *lexer) {
  char *ret = (char *)malloc(1);
  ret[0] = '\0';
  char merge[2];
  merge[1] = '\0';
  while (is_valid_id_char(lexer->c)) {
    ret = realloc(ret, (strlen(ret) + 2));
    merge[0] = lexer->c;
    strcat(ret, merge);
    lexer_move(lexer);
  }
  return init_token(TOKEN_ID, ret, lexer->row, lexer->col);
}

token_t *lexer_collect_num(lexer_t *lexer) {
  char *ret = (char *)malloc(1);
  ret[0] = '\0';
  bool is_float = false;
  char merge[2];
  merge[1] = '\0';
  while (isdigit(lexer->c) || (lexer->c == '.' && !is_float)) {
    if (lexer->c == '.')
      is_float = true;
    merge[0] = lexer->c;
    ret = realloc(ret, (strlen(ret) + 2));
    strcat(ret, merge);
    lexer_move(lexer);
  }
  if (is_float)
    return init_token(TOKEN_FLOAT, ret, lexer->row, lexer->col);
  return init_token(TOKEN_INT, ret, lexer->row, lexer->col);
}

token_t *lexer_collect_string(lexer_t *lexer) {
  char *ret = (char *)malloc(1);
  ret[0] = '\0';
  char merge[2];
  merge[1] = '\0';
  lexer_move(lexer);
  while (lexer->c != '"') {
    ret = realloc(ret, (strlen(ret) + 2));
    merge[0] = lexer->c;
    strcat(ret, merge);
    lexer_move(lexer);
  }
  lexer_move(lexer);
  return init_token(TOKEN_STRING, ret, lexer->row, lexer->col);
}

static token_t *lexer_move_with(lexer_t *lexer, token_t *token) {
  lexer_move(lexer);
  return token;
}

token_t *lexer_collect_next(lexer_t *lexer) {
start:
  if (lexer->c == '\0' || lexer->c == EOF) {
    lexer->finished = true;
    return NULL;
  }
  if (isspace(lexer->c)) {
    lexer_ignore_whitespace(lexer);
    goto start;
  }
  if (lexer->c == ';') {
    lexer_skip_comment(lexer);
    goto start;
  }

  if (isdigit(lexer->c))
    return lexer_collect_num(lexer);
  else if (is_valid_id_char(lexer->c))
    return lexer_collect_id(lexer);
  else if (lexer->c == '"')
    return lexer_collect_string(lexer);
  else if (lexer->c == '#')
    return lexer_collect_bool(lexer);
  else if (lexer->c == '(')
    return lexer_move_with(
        lexer, init_token(TOKEN_LPAREN, "(", lexer->row, lexer->col));
  else if (lexer->c == ')')
    return lexer_move_with(
        lexer, init_token(TOKEN_RPAREN, ")", lexer->row, lexer->col));
  else if (lexer->c == '\'')
    return lexer_move_with(
        lexer, init_token(TOKEN_QUOTE, "'", lexer->row, lexer->col));
  else if (lexer->c == '.')
    return lexer_move_with(
        lexer, init_token(TOKEN_PERIOD, ".", lexer->row, lexer->col));
  else {
    return NULL;
  }
}
