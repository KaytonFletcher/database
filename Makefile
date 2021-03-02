CC = g++ -std=c++17 -fPIE -Wno-deprecated 

PROD_FLAGS = -O2
DEV_FLAGS = -g -Wall 

CORE_FILES = ./db_file/*.cc ./db_core/*.cc ./compare/*.cc y.tab.o lex.yy.o 

tag = -i

ifdef linux
tag = -n
endif

dev.out: y.tab.o lex.yy.o 
	$(CC) -o dev.out $(DEV_FLAGS) $(CORE_FILES) ./p2_part2/test.cc -lfl -lpthread

gtests.out: y.tab.o lex.yy.o 
	$(CC) -g -o gtests.out $(PROD_FLAGS) $(CORE_FILES) ./gtests/main.cc -lfl -lgtest -lpthread

test.out: y.tab.o lex.yy.o 
	$(CC) -o test.out $(PROD_FLAGS) $(CORE_FILES) ./p2_part2/test.cc -lfl -lpthread

test2_1.out: y.tab.o lex.yy.o 
	$(CC) -o test2_1.out $(PROD_FLAGS) $(CORE_FILES) ./p2_part1/test.cc -lfl -lpthread

test1.out: y.tab.o lex.yy.o 
	$(CC) -o test1.out $(PROD_FLAGS) $(CORE_FILES) ./p1/test.cc -lfl -lpthread
	
y.tab.o: dependencies/Parser.y
	yacc -d dependencies/Parser.y
	sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c

lex.yy.o: dependencies/Lexer.l
	lex  dependencies/Lexer.l
	gcc  -c lex.yy.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h