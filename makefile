all:
	flex ijparser.l
	yacc --defines=y.tab.h ijparser.y
	gcc -o llvm *.c -lfl -ly -g
