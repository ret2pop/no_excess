#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "./ast.h"

typedef struct {
  char *key;
  ast_t *value;
} pair_t;

typedef struct SL_NODE_STRUCT {
  pair_t *value;
  struct SL_NODE_STRUCT *next;
} sl_node_t;

typedef struct {
  sl_node_t *head;
  int size;
} sl_list_t;

typedef struct {
  int size;
  sl_list_t **buckets;
} hash_table_t;

pair_t *init_pair(char *key, ast_t *value);

sl_node_t *init_sl_node(char *key, ast_t *value);

sl_list_t *init_sl_list();

void sl_list_add(sl_list_t *l, char *key, ast_t *value);

ast_t *sl_list_get(sl_list_t *l, char *key);

bool sl_list_exists(sl_list_t *l, char *key);

void sl_list_free(sl_list_t *l);

hash_table_t *init_hash_table(int size);

void hash_table_add(hash_table_t *h, char *key, ast_t *value);

ast_t *hash_table_get(hash_table_t *h, char *key);

bool hash_table_exists(hash_table_t *h, char *key);

unsigned long hash(char *key, int size);

void hash_table_free(hash_table_t *h);

#endif
