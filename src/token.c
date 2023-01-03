#include "./include/token.h"
#include "./include/macros.h"

#include <stdlib.h>

token_t *init_token(int type, char *value, int row, int col) {
  token_t *t = (token_t *)malloc(sizeof(token_t));
  if (t == NULL)
    die("malloc on token struct");
  t->type = type;
  t->value = value;
  t->row = row;
  t->col = col;
  return t;
}
