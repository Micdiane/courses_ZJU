CC=gcc
CFLAGS=-Wall -Wextra

all: myprogram

myprogram: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o myprogram lex.yy.c y.tab.c tree.c tree.h -lfl 

lex.yy.c: lexer.l
	flex lexer.l

y.tab.c: parser.y
	bison -y -d parser.y

clean:
	rm -f lex.yy.c y.tab.c y.tab.h myprogram

