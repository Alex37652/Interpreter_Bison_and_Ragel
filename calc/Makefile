run: calc
	./calc < program.in > test.out
	diff test.out program.out

calc: parser.cc lexer.cc lexer.h main.cc ast.cc eval.cc
	g++ --std=c++17 -g -o calc main.cc parser.cc lexer.cc ast.cc eval.cc

lexer.cc: lexer.l
	ragel -o lexer.cc lexer.l

parser.cc: parser.y
	bison -o parser.cc parser.y

clean:
	rm -f calc lexer.cc parser.cc parser.hh stack.hh
