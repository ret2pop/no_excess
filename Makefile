CC = gcc
CFLAGS  = -g -Wall

nxs: src/ast.c src/hash_table.c src/lexer.c src/macros.c src/main.c src/parser.c src/print.c src/stack.c src/token.c src/visitor.c
	cc src/*.c $(CFLAGS) -o nxs

clean:
	-rm nxs

install:
	cp nxs /usr/local/bin/

uninstall:
	-rm /usr/local/bin/nxs
