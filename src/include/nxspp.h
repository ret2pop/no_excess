#ifndef NXSPP_H
#define NXSPP_H

typedef struct {
  char *source;
  char c;
  int i;
} npp_t;

npp_t *init_npp(char *source);
char *process_string(npp_t *p);
#endif
