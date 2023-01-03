#define _GNU_SOURCE
#include "./include/ast.h"
#include "./include/hash_table.h"
#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/print.h"
#include "./include/stack.h"
#include "./include/token.h"
#include "./include/visitor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  /* Test Lexer */
  /* lexer_t *lexer = init_lexer("'(fasd \"aaaaaaaa\" 4)"); */
  /* token_t *t = lexer_collect_next(lexer); */
  /* while (t != NULL) { */
  /*   printf("%d: %s\n", t->type, t->value); */
  /*   t = lexer_collect_next(lexer); */
  /* } */
  /* printf("Welcome to the NXS REPL.\n"); */

  /* char *buf = malloc(2); */
  /* size_t size = 2; */
  /* lexer_t *lexer; */
  /* lexer = init_lexer("a"); */
  /* token_t *t; */
  /* while (true) { */
  /*   printf("> "); */
  /*   fflush(stdout); */
  /*   getline(&buf, &size, stdin); */
  /*   strcat(buf, "\0"); */
  /*   lexer_reset(lexer, buf); */
  /*   t = lexer_collect_next(lexer); */
  /*   while (!lexer->finished) { */
  /*     printf("%d\t%s\n", t->type, t->value); */
  /*     t = lexer_collect_next(lexer); */
  /*     printf("lmao\n"); */
  /*   } */
  /* } */

  /* TEST PARSER, VISITOR, PRINTER */
  lexer_t *lexer = init_lexer("34.4");
  parser_t *parser = init_parser(lexer);

  ast_t *root = parse_expr(parser);

  visitor_t *v = init_visitor(root);

  ast_t *res = eval_expr(v, root);

  print(res);
  return 0;
}
