#ifndef SL_LIST_H
#define SL_LIST_H

typedef struct SL_NODE_STRUCT {
  void *p1;
  void *p2;
} sl_node_t;

typedef struct {
  sl_node_t *head;
  int size;
} sl_list_t;

sl_node_t *init_sl_node(void *value);

sl_list_t *init_sl_list();

void sl_list_add(sl_list_t *l, void *value, int i);

void sl_list_del(sl_list_t *l, int i);

void *sl_list_get(sl_list_t *l, int i);
#endif
