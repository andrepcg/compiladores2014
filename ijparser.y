%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"
#include "shows.h"


extern int prevLineNo;
extern int prevColNo;
extern int yyleng;
extern char *yytext;

int yydebug=0;

void yyerror(char *s);
int yylex(void);

%}

%union{
	char *token;
}


%token RESERVED COMMA BOOL INT ID INTLIT IF ELSE WHILE RETURN PRINT BOOLLIT NEW PARSEINT PUBLIC STATIC VOID 
%token CLASS OCURV CCURV OBRACE CBRACE OSQUARE CSQUARE OP1 OP2 OP3 OP4 NOT ASSIGN SEMIC STRING DOTLENGTH

%nonassoc EXPR1REDUCE
%nonassoc IF ELSE

%right ASSIGN
%left OP1
%left OP2
%left OP3
%left OP4
%left COMMA
%right NOT
%left OSQUARE DOTLENGTH
%right UNARY

%start start

%% 

start
	:	program
	;

program
	:	CLASS ID OBRACE CBRACE
	|	CLASS ID OBRACE program_multi CBRACE
	;

field_method_decl
	:	field_decl
	|	method_decl
	;
	
program_multi
	:	field_method_decl
	|	program_multi field_method_decl
	;

field_decl
	:	STATIC var_decl
	;
	
method_decl
	:	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE multi_var_decl statement_multi CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE multi_var_decl CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE statement_multi CBRACE
	;
	
type_void
	:	type
	|	VOID
	;

formal_params
	:	type ID
	|	type ID comma_type_id_multi
	|	STRING OSQUARE CSQUARE ID
	|
	;
	
comma_type_id_multi
	:	COMMA type ID
	|	comma_type_id_multi COMMA type ID
	;
	
type
	:	int_bool
	|	int_bool OSQUARE CSQUARE
	;
	
multi_var_decl
	:	var_decl
	|	multi_var_decl var_decl
	;

	
int_bool
	:	INT
	|	BOOL
	;
	
var_decl
	:	type ID SEMIC
	|	type ID comma_id_multi SEMIC
	
comma_id_multi
	:	COMMA ID
	|	comma_id_multi COMMA ID
	;
	
statement
	:	OBRACE CBRACE
	|	OBRACE statement_multi CBRACE
	|	IF OCURV expr CCURV statement
	|	IF OCURV expr CCURV statement ELSE statement
	|	WHILE OCURV expr CCURV statement
	|	PRINT OCURV expr CCURV SEMIC
	|	ID ASSIGN expr SEMIC
	|	ID OSQUARE expr CSQUARE ASSIGN expr SEMIC
	|	RETURN SEMIC
	|	RETURN expr SEMIC
	;
	
statement_multi
	:	statement
	|	statement_multi statement
	;
	
expr
	:	exprindex	%prec EXPR1REDUCE			  					
	|	exprindex OSQUARE expr CSQUARE  				                        
	|	exprnotindex	
	
	
exprindex
	:	expr OP1 expr    
	|	expr OP2 expr
  	|	expr OP3 expr
  	|	expr OP4 expr
    |	id_int_bool                         
    |	OCURV expr CCURV                    
    |	expr DOTLENGTH                  
    |	NOT expr          %prec UNARY  
	|	OP3 expr
    |	PARSEINT OCURV ID OSQUARE expr CSQUARE CCURV
    |	ID OCURV CCURV
	|	ID OCURV args CCURV
	;

exprnotindex
	:	NEW int_bool OSQUARE expr CSQUARE      
    ;

	
id_int_bool
	:	ID
	|	INTLIT
	|	BOOLLIT
	;

args
	:	expr
	|	expr comma_expr_multi
	;
	
comma_expr_multi
	:	COMMA expr
	|	comma_expr_multi COMMA expr
	;

	
%%

int main(int argc, char *argv[])
{
	yyparse();

	int i, printTree, printSymbols;
	printTree = printSymbols = 0;
	for(i=0; i < argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0){
			printSymbols = 1;
			break;
		}
		else if(strcmp(argv[i], "-t") == 0){
			printTree = 1;
			break;
		}
	}
	
	/*
	if(printTree)
		printProgram(myProgram);
	if(printSymbols)
		printSymbolTables(symbolsTable);*/

	//freeProgram(myProgram, symbolsTable);
    return 0;
}

void yyerror(char *s) {
	printf("Line %d, col %d: %s: %s\n", prevLineNo, prevColNo, s, yytext);
}
