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
  /* DONE: TEST LEXER */
  /* lexer_t *lexer = init_lexer("'(fasd asdf)"); */
  /* token_t *t = lexer_collect_next(lexer); */
  /* while (t != NULL) { */
  /*   printf("%d: %s\n", t->type, t->value); */
  /*   t = lexer_collect_next(lexer); */
  /* } */

  /* TODO: TEST REPL POSSIBILITY */
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

  /* DONE: TEST PARSER, VISITOR, PRINTER (self evaluating types) */
  /* lexer_t *lexer = init_lexer("\"hello world\""); */
  /* parser_t *parser = init_parser(lexer); */
  /* visitor_t *visitor = init_visitor(parser); */
  /* ast_t *root = eval(visitor); */
  /* print_root(root); */

  /* DONE: TEST PARSING LISTS */
  /* lexer_t *lexer = init_lexer("(hello (world #f))"); */
  /* parser_t *parser = init_parser(lexer); */
  /* ast_t *root = parse_all(parser); */
  /* ast_t *list = root->subnodes[0]; */
  /* ast_t *hello = list->car; */
  /* ast_t *inner = list->cdr->car; */
  /* ast_t *world = inner->car; */
  /* ast_t *fal = inner->cdr->car; */
  /* printf("%s\n", hello->string_value); */
  /* printf("%s\n", world->string_value); */
  /* printf("%d\n", fal->bool_value); */

  /* TODO: TEST PARSING FUNCTIONS */
  lexer_t *lexer = init_lexer("(lambda (x) (y))");
  printf("why is code not working, part 1\n");
  parser_t *parser = init_parser(lexer);
  printf("why is code not working, part 2\n");
  ast_t *root = parse_all(parser);

  ast_t *func = root->subnodes[0];
  printf("%d\n", func->type);
  /* DONE: TEST PARSING QUOTE */
  /* lexer_t *lexer = init_lexer("'(hello)"); */
  /* parser_t *parser = init_parser(lexer); */
  /* ast_t *root = parse_all(parser); */
  /* ast_t *quote = root->subnodes[0]; */
  /* printf("%s\n", quote->cdr->car->car->string_value); */
  return 0;
}
