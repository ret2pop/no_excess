#include "./include/macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die(char *message) {
  fprintf(stderr, "%sFATAL ERROR: %s%s\n", RED, message, reset);
  exit(EXIT_FAILURE);
}
