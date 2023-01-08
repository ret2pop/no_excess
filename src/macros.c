#include "./include/macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die(char *message) {
  fprintf(stderr, "%sFATAL ERROR: %s%s\n", RED, message, reset);
  exit(EXIT_FAILURE);
}

char *char_to_string(char c) {
  char *s = malloc(2);
  s[0] = c;
  s[1] = '\0';
  return s;
}
