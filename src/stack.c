#include "./include/stack.h"
#include "./include/hash_table.h"
#include "./include/macros.h"
#include <stdlib.h>

stack_t *init_stack(int ht_size) {
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
      die("malloc on stack within stack");
  } else {
    s->stack = realloc(s->stack, 2 + s->cur);
  }
  s->cur++;
  s->stack[s->cur] = h;
}

hash_table_t *stack_peek(stack_t *s) { return s->stack[s->cur]; }

hash_table_t *stack_pop(stack_t *s) {
  hash_table_t *h = s->stack[s->cur];
  s->stack[s->cur] = NULL;
  s->cur--;
  return h;
}
