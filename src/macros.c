#include "./include/macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die(char *message) {
  fprintf(stderr, "%sFATAL ERROR: %s%s\n", RED, message, reset);
  exit(EXIT_FAILURE);
}

char *char_to_string(char c) {
  char *str = malloc(2);
  str[0] = c;
  str[1] = '\0';
  return str;
}
