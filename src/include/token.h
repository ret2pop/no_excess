#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
  enum {
    TOKEN_ID,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_BOOL,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_QUOTE,
    TOKEN_PERIOD,
  } type;
  char *value;
  int row;
  int col;
} token_t;

token_t *init_token(int type, char *value, int row, int col);
#endif
