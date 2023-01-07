#include "./include/nxspp.h"
#include "./include/macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

npp_t *init_npp(char *source) {
  npp_t *n = malloc(sizeof(npp_t));
  if (n == NULL)
    die("malloc on nxspp");
  n->source = source;
  n->i = 0;
  n->c = source[n->i];
  return n;
}

char *nxspp_collect_id(char *source, int i) {}

void npp_move(npp_t *p) {
  if (p->c != '\0') {
    p->i++;
    p->c = p->source[p->i];
  }
}
