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
  /* TEST LEXER */
  /* lexer_t *lexer = init_lexer("'(fasd asdf)"); */
  /* token_t *t = lexer_collect_next(lexer); */
  /* while (t != NULL) { */
  /*   printf("%d: %s\n", t->type, t->value); */
  /*   t = lexer_collect_next(lexer); */
  /* } */

  /* TEST REPL POSSIBILITY */
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

  /* TEST PARSER, VISITOR, PRINTER (self evaluating types) */
  /* lexer_t *lexer = init_lexer("\"hello world\""); */
  /* parser_t *parser = init_parser(lexer); */
  /* visitor_t *visitor = init_visitor(parser); */
  /* ast_t *root = eval(visitor); */
  /* print_root(root); */

  /* TEST PARSING LISTS */
  lexer_t *lexer = init_lexer("(hello world)");
  parser_t *parser = init_parser(lexer);
  ast_t *root = parse_all(parser);
  ast_t *list = root->subnodes[0];
  ast_t *hello = list->car;
  ast_t *world = list->cdr->car;
  printf("%s\n", hello->string_value);
  printf("%s\n", world->string_value);

  /* ast_t *e2 = list->cdr->car; */
  /* if (e2 == NULL) { */
  /*   printf("something is wrong here...\n"); */
  /*   exit(0); */
  /* } */

  /* printf("%s\n", e2->string_value); */
  /* printf("number 3\n"); */
  return 0;
}
