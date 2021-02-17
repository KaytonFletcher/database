CC = g++ -fPIE -O2 -Wno-deprecated 

tag = -i

ifdef linux
tag = -n
endif

gtests.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o CNF.o OrderMaker.o BigQ.o Pipe.o y.tab.o lex.yy.o gtests.o
	$(CC) -I . -I ./gtests -o gtests.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o CNF.o OrderMaker.o BigQ.o Pipe.o y.tab.o lex.yy.o gtests.o -lfl -lgtest -lpthread

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o CNF.o OrderMaker.o BigQ.o Pipe.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o CNF.o OrderMaker.o BigQ.o Pipe.o y.tab.o lex.yy.o test.o -lfl -lpthread
	
main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o CNF.o OrderMaker.o y.tab.o lex.yy.o main.o
	$(CC) -o main.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o CNF.o OrderMaker.o y.tab.o lex.yy.o main.o -lfl

gtests.o: gtests/main.cc
	$(CC) -g -o gtests.o -c gtests/main.cc

test.o: test.cc
	$(CC) -g -c test.cc

main.o: main.cc
	$(CC) -g -c main.cc
	
Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc
	
ComparisonEngine.o: ComparisonEngine.cc
	$(CC) -g -c ComparisonEngine.cc
	
DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc

File.o: File.cc
	$(CC) -g -c File.cc

Record.o: Record.cc
	$(CC) -g -c Record.cc

Schema.o: Schema.cc
	$(CC) -g -c Schema.cc

CNF.o: CNF.cc
	$(CC) -g -c CNF.cc

OrderMaker.o: OrderMaker.cc
	$(CC) -g -c OrderMaker.cc

BigQ.o: BigQ.cc
	$(CC) -g -c BigQ.cc
	
Pipe.o: Pipe.cc
	$(CC) -g -c Pipe.cc
	
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c

lex.yy.o: Lexer.l
	lex  Lexer.l
	gcc  -c lex.yy.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h