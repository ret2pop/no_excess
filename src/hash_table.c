#include "./include/hash_table.h"
#include "./include/ast.h"
#include "./include/macros.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pair_t *init_pair(char *key, ast_t *value) {
  pair_t *p = (pair_t *)malloc(sizeof(pair_t));
  if (p == NULL)
    die("malloc on pair");
  p->key = key;
  p->value = value;
  return p;
}

sl_node_t *init_sl_node(char *key, ast_t *value) {
  sl_node_t *n = (sl_node_t *)malloc(sizeof(sl_node_t));
  if (n == NULL)
    die("malloc on node");
  n->value = init_pair(key, value);
  n->next = NULL;
  return n;
}

void sl_node_free(sl_node_t *n) {
  ast_free(n->value->value);
  free(n->value);
  free(n);
}
/*** SINGLY LINKED LIST FUNCTIONS ***/
sl_list_t *init_sl_list() {
  sl_list_t *l = (sl_list_t *)malloc(sizeof(sl_list_t));
  if (l == NULL)
    die("malloc on list");
  l->size = 0;
  l->head = NULL;
  return l;
}

/* TODO: fix segfault bug */
/* void sl_list_add(sl_list_t *l, char *key, ast_t *value) { */
/*   sl_node_t *cur = l->head; */
/*   bool modified = false; */
/*   if (l->head == NULL) { */
/*     l->head = init_sl_node(key, value); */
/*     l->size++; */
/*     return; */
/*   } */

/*   for (int i = 0; i < l->size - 1; i++) { */
/*     if (strcmp(cur->value->key, key) == 0) { */
/*       cur->value->value = value; */
/*       modified = true; */
/*       break; */
/*     } */
/*     cur = cur->next; */
/*   } */

/*   if (strcmp(cur->value->key, key) == 0) { */
/*     cur->value->value = value; */
/*     modified = true; */
/*   } */

/*   if (!modified) { */
/*     cur->next = init_sl_node(key, value); */
/*     l->size++; */
/*   } */
/* } */

void sl_list_add(sl_list_t *l, char *key, ast_t *value) {
  sl_node_t *cur = l->head;
  if (cur == NULL) {
    l->head = init_sl_node(key, value);
    l->size++;
  } else {
    while (cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = init_sl_node(key, value);
    l->size++;
  }
}

void sl_list_modify(sl_list_t *l, char *key, ast_t *value) {
  sl_node_t *cur = l->head;
  while (cur != NULL) {
    if (strcmp(cur->value->key, key) == 0)
      cur->value->value = value;
    cur = cur->next;
  }
}

ast_t *sl_list_get(sl_list_t *l, char *key) {
  sl_node_t *cur = l->head;
  for (int i = 0; i < l->size; i++) {
    if (cur == NULL)
      return NULL;
    if (strcmp(cur->value->key, key) == 0)
      return cur->value->value;
    cur = cur->next;
  }
  return NULL;
}

bool sl_list_exists(sl_list_t *l, char *key) {
  if (sl_list_get(l, key) != NULL)
    return true;
  return false;
}

void sl_list_free(sl_list_t *l) {
  sl_node_t *cur = l->head;
  sl_node_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = tmp->next;
    sl_node_free(tmp);
  }
  free(l);
}

void sl_list_free_some(sl_list_t *l) {
  sl_node_t *cur = l->head;
  sl_node_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = tmp->next;
    free(tmp);
  }
  free(l);
}
/*** HASH TABLE FUNCTIONS ***/
hash_table_t *init_hash_table(int size) {
  hash_table_t *h = (hash_table_t *)malloc(sizeof(hash_table_t));
  if (h == NULL)
    die("malloc on hash table");
  h->size = size;
  h->buckets = malloc(size * sizeof(sl_list_t *));
  if (h->buckets == NULL)
    die("malloc on buckets");
  for (int i = 0; i < h->size; i++)
    h->buckets[i] = init_sl_list();
  return h;
}

void hash_table_add(hash_table_t *h, char *key, ast_t *value) {
  sl_list_t *l = h->buckets[hash(key, h->size)];
  if (sl_list_exists(l, key))
    sl_list_modify(l, key, value);
  sl_list_add(l, key, value);
}

ast_t *hash_table_get(hash_table_t *h, char *key) {
  sl_list_t *l = h->buckets[hash(key, h->size)];
  return sl_list_get(l, key);
}

bool hash_table_exists(hash_table_t *h, char *key) {
  sl_list_t *l = h->buckets[hash(key, h->size)];
  return sl_list_exists(l, key);
}

void hash_table_free(hash_table_t *h) {
  for (int i = 0; i < h->size; i++)
    sl_list_free(h->buckets[i]);
  free(h);
}

void hash_table_free_some(hash_table_t *h) {
  for (int i = 0; i < h->size; i++)
    sl_list_free_some(h->buckets[i]);
  free(h);
}
/* DJB2 HASH FUNCTION */
unsigned long hash(char *key, int size) {
  unsigned long hash = 5381;
  int c;
  while ((c = *key++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash % size;
}
