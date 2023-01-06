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

// Read the file into allocated memory.
// Return NULL on error.

int main(int argc, char **argv) {
  /* DONE: TEST LEXER */
  /* lexer_t *lexer = init_lexer("'(fasd asdf)"); */
  /* token_t *t = lexer_collect_next(lexer); */
  /* while (t != NULL) { */
  /*   printf("%d: %s\n", t->type, t->value); */
  /*   t = lexer_collect_next(lexer); */
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

  /* DONE: TEST PARSING FUNCTIONS */
  /* lexer_t *lexer = init_lexer("(lambda (x) (y))"); */
  /* parser_t *parser = init_parser(lexer); */
  /* ast_t *root = parse_all(parser); */
  /* ast_t *func = root->subnodes[0]; */
  /* ast_t *args = func->car; */
  /* ast_t *expr = func->cdr; */
  /* printf("%d\t%s\t\%s\n", func->type, args->car->string_value, */
  /*        expr->car->string_value); */

  /* DONE: TEST PARSING QUOTE */
  /* lexer_t *lexer = init_lexer("'(hello)"); */
  /* parser_t *parser = init_parser(lexer); */
  /* ast_t *root = parse_all(parser); */
  /* ast_t *quote = root->subnodes[0]; */
  /* printf("%s\n", quote->cdr->car->car->string_value); */

  /* DONE: TEST PARSING AND STORING BINDINGS */
  /* lexer_t *lexer = init_lexer("(bind x \"hello world\")"); */
  /* parser_t *parser = init_parser(lexer); */
  /* parse_all(parser); */
  /* if (hash_table_exists(parser->symbol_table, "x")) { */
  /*   printf("YES!\n"); */
  /*   ast_t *str = hash_table_get(parser->symbol_table, "x"); */
  /*   printf("%s\n", str->string_value); */
  /* } */

  /* TODO: TEST HASH TABLE COLLISIONS */
  /* DONE: TEST BUILTIN FUNCTIONS */
  /* lexer_t *lexer = init_lexer("(* (+ 3.0 4.0) 4)"); */
  /* parser_t *parser = init_parser(lexer); */
  /* visitor_t *visitor = init_visitor(parser); */
  /* ast_t *root = eval(visitor); */
  /* ast_t *res = root->subnodes[0]; */
  /* print(res); */

  /* DONE: TEST NON-BUILTIN FUNCTIONS (stack frame) */
  /* lexer_t *lexer = init_lexer("(if (= (+ 3 2) 5) 1 2)"); */
  /* parser_t *parser = init_parser(lexer); */
  /* visitor_t *visitor = init_visitor(parser); */
  /* ast_t *root = eval(visitor); */
  /* print_root(root); */

  /* DONE: TEST RECURSION */
  /* char *expr = "(if (= 0 0) 1 2)"; */
  /* char *expr = "(bind factorial (lambda (n) (if (= n 0) 1 (* n (factorial (-
   * n " */
  /*              "1)))))) (factorial 3)"; */
  /* lexer_t *lexer = init_lexer(expr); */
  /* parser_t *parser = init_parser(lexer); */
  /* visitor_t *visitor = init_visitor(parser); */
  /* ast_t *root = eval(visitor); */
  /* print_root(root); */

  /*   lexer_t *lexer = init_lexer("((lambda (x y) (+ x y)) (+ 3 4) 4) (+ 3
   * 4)"); */
  /*   printf("debug 1\n"); */
  /*   parser_t *parser = init_parser(lexer); */
  /*   printf("debug 2\n"); */
  /*   visitor_t *visitor = init_visitor(parser); */
  /*   printf("debug 3\n"); */
  /*   ast_t *root = eval(visitor); */
  /*   printf("debug 4\n"); */
  /*   print_root(root); */
  /*   printf("debug 5\n"); */
  /*   ast_t *res = root->subnodes[0]; */
  /*   print(res); */

  /* TODO: TEST REPL POSSIBILITY */
  /* printf("Welcome to the NXS REPL.\n"); */

  /* char *buf = malloc(2); */
  /* size_t size = 2; */
  /* lexer_t *lexer; */
  /* lexer = init_lexer("a"); */
  /* while (true) { */
  /*   printf("> "); */
  /*   fflush(stdout); */
  /*   getline(&buf, &size, stdin); */
  /*   lexer_reset(lexer, buf); */
  /* } */

  /* DONE: BE ABLE TO READ IN FILE AS INPUT */
  if (argc < 2) {
    printf("TOO FEW ARGUMENTS.\n");
    exit(1);
  }
  char *filename = argv[1];
  char *buffer = 0;
  long length;
  FILE *f = fopen(filename, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  }

  if (buffer) {
    /* printf("%s", buffer); */
    lexer_t *lexer = init_lexer(buffer);
    parser_t *parser = init_parser(lexer);
    visitor_t *visitor = init_visitor(parser);
    ast_t *root = eval(visitor);
    print_root(root);
  }

  return 0;
}
