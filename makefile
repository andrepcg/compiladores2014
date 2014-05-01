all:
	lex ijparser.l
	yacc --defines=y.tab.h ijparser.y
	gcc -o ijparser *.c -ll -ly -g
