#include "./include/stack.h"
#include "./include/hash_table.h"
#include "./include/macros.h"
#include <stdio.h>
#include <stdlib.h>

stack_t *init_stack() {
  stack_t *s = (stack_t *)malloc(sizeof(stack_t));
  if (s == NULL)
    die("malloc on stack");
  s->stack = NULL;
  s->cur = -1;
  return s;
}

void stack_push(stack_t *s, hash_table_t *h) {
  if (s->stack == NULL) {
    s->stack = malloc(sizeof(hash_table_t *));
    if (s->stack == NULL)
      die("malloc on stack within stack_push");
  } else {
    s->stack = realloc(s->stack, (2 + s->cur) * sizeof(hash_table_t *));
  }
  s->cur++;
  s->stack[s->cur] = h;
}
/* fix heap buffer overflow */
hash_table_t *stack_peek(stack_t *s) {
  if (is_empty(s))
    return NULL;
  return s->stack[s->cur];
}

hash_table_t *stack_pop(stack_t *s) {
  hash_table_t *h = s->stack[s->cur];
  s->stack[s->cur] = NULL;
  s->cur--;
  return h;
}

bool is_empty(stack_t *s) {
  if (s->cur == -1)
    return true;
  return false;
}
