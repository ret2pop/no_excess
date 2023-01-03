#ifndef STACK_H
#define STACK_H
#include "./hash_table.h"
typedef struct {
  hash_table_t **stack;
  int cur;
} stack_t;

stack_t *init_stack(int ht_size);

void stack_push(stack_t *s, hash_table_t *h);

hash_table_t *stack_peek(stack_t *s);

hash_table_t *stack_pop(stack_t *s);
#endif
