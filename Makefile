CC      = gcc
CFLAGS  = -Wall -Wextra -g
OBJS    = main.o ast.o bcs24.tab.o lex.yy.o

all: bcs24

bcs24: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

bcs24.tab.c bcs24.tab.h: bcs24.y
	bison -d bcs24.y

lex.yy.c: bcs24.l bcs24.tab.h
	flex bcs24.l

main.o ast.o bcs24.tab.o lex.yy.o: ast.h
lex.yy.o main.o: bcs24.tab.h

test: bcs24
	sh ./run_tests.sh

dist:
	tar czf bcs24_stage1.tar.gz bcs24.l bcs24.y ast.h ast.c main.c Makefile run_tests.sh tests

clean:
	rm -f bcs24 *.o lex.yy.c bcs24.tab.c bcs24.tab.h bcs24_stage1.tar.gz

.PHONY: all test dist clean
