#ifndef PRINT_H
#define PRINT_H
#include "./ast.h"

void print_string(ast_t *str);

void print_int(ast_t *i);

void print_bool(ast_t *b);

void print_float(ast_t *f);

void print_func(ast_t *f);

void print(ast_t *res);
#endif
