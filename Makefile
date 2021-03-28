CC = g++ -std=c++17 -fPIE -Wno-deprecated 

PROD_FLAGS = -O2
DEV_FLAGS = -g -pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

CORE_FILES = ./db_file/*.cc ./db_core/*.cc ./compare/*.cc relational_ops/*.cc y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o

tag = -i

ifdef linux
tag = -n
endif

dev.out: y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o
	$(CC) -o dev.out $(DEV_FLAGS) $(CORE_FILES) ./p3/test.cc -lfl -lpthread

gtests.out: y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o
	$(CC) -g -o gtests.out $(DEV_FLAGS) $(CORE_FILES) ./gtests/main.cc -lfl -lgtest -lpthread

test.out: y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o
	$(CC) -o test.out $(PROD_FLAGS) $(CORE_FILES) ./p3/test.cc -lfl -lpthread

test2_2.out: y.tab.o lex.yy.o 
	$(CC) -o test2_1.out $(PROD_FLAGS) $(CORE_FILES) ./p2_part2/test.cc -lfl -lpthread

test2_1.out: y.tab.o lex.yy.o 
	$(CC) -o test2_1.out $(PROD_FLAGS) $(CORE_FILES) ./p2_part1/test.cc -lfl -lpthread

test1.out: y.tab.o lex.yy.o 
	$(CC) -o test1.out $(PROD_FLAGS) $(CORE_FILES) ./p1/test.cc -lfl -lpthread
	
y.tab.o: dependencies/Parser.y
	yacc -d dependencies/Parser.y
	#sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c
		
yyfunc.tab.o: dependencies/ParserFunc.y
	yacc -p "yyfunc" -b "yyfunc" -d dependencies/ParserFunc.y
	#sed $(tag) yyfunc.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c yyfunc.tab.c
	
lex.yy.o: dependencies/Lexer.l
	lex dependencies/Lexer.l
	gcc  -c lex.yy.c

lex.yyfunc.o: dependencies/LexerFunc.l
	lex -Pyyfunc dependencies/LexerFunc.l
	gcc  -c lex.yyfunc.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.*
	rm -f yyfunc.tab.*
	rm -f lex.yy.*
	rm -f lex.yyfunc*