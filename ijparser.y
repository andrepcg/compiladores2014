%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"
#include "functions.h"
#include "shows.h"
#include "symbol_table.h"

extern int linhacount;
extern int colcount;
extern int yyleng;
extern char *yytext;

int yydebug=0;

void yyerror(char *s);
int yylex(void);

is_node *myProgram;
prog_env *myProgramSemantic;
%}


%token RESERVED COMMA BOOL INT ID INTLIT IF ELSE WHILE RETURN PRINT BOOLLIT NEW PARSEINT PUBLIC STATIC VOID 
%token CLASS OCURV CCURV OBRACE CBRACE OSQUARE CSQUARE OP1 OP2 OP3 OP4 NOT ASSIGN SEMIC STRING DOTLENGTH

%token <valorInteiro> INTLIT
%token <valorID> ID

%type <node> 

%union{
	int valorInteiro;
	char* valorID;
	is_node *node;
}

%right ASSIGN
%left OP1 
%left OP3
%left OP4
%left OP2
%right NOT
%left OCURV CCURV
%left OBRACE CBRACE
%left OSQUARE CSQUARE
%nonassoc IF ELSE

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
	:	PUBLIC STATIC type_void ID OCURV CCURV OBRACE 
	|	PUBLIC STATIC type_void ID OCURV formal_params OBRACE CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params OBRACE var_decl statement CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params OBRACE var_decl CBRACE
	|	PUBLIC STATIC type_void ID OCURV formal_params OBRACE statement CBRACE
	;
	
type_void
	:	type
	|	VOID
	;

formal_params
	:	type ID comma_type_id_multi
	|	STRING OSQUARE CSQUARE ID
	;
	
comma_type_id_multi
	:	COMMA type ID
	|	comma_type_id_multi COMMA type ID
	;
	
type
	:	int_bool square_multi
	;
	
square_multi
	:	OSQUARE CSQUARE
	|	square_multi OSQUARE CSQUARE
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
	|	ID ASSING expr SEMIC
	|	ID OSQUARE espr CSQUARE ASSING expr SEMIC
	|	RETURN SEMIC
	|	RETURN expr SEMIC
	;
	
statement_multi
	:	statement
	|	statement_multi statement
	;
	
expr
	: 	expr op_or expr
	|	expr OSQUARE expr CSQUARE
	|	ID
	|	INTLIT
	|	BOOLLIT
	|	NEW int_bool OSQUARE expr CSQUARE
	|	OCURV expr CCURV
	|	expr DOTLENGTH expr
	|	expr op3_not expr	
	|	PARSEINT OCURV ID OSQUARE expr CSQUARE CCURV
	|	ID OCURV CCURV
	|	ID OCURV args CCURV
	;

args
	:	expr
	|	expr comma_expr_multi
	;
	
comma_expr_multi
	:	COMMA expr
	|	comma_expr_multi COMMA expr
	;

op3_not
	:	OP3
	|	NOT
	;
	
op_or
	:	OP1
	|	OP2
	|	OP3
	|	OP4
	;

	
%%

int main(){
	/*
	int i, show_tree = 0, show_semantic=0;
	
	if(yyparse()==0){
		for(i=0; i<argc; i++){
			if(strcmp(argv[i], "-t") == 0){
				show_tree = 1;
			} else if(strcmp(argv[i], "-s") == 0) {
				show_semantic = 1;
			}

		}

		if(show_tree == 1){
			printf("Program\n");
			show_program(myProgram, 1);
		} 
		if( show_semantic == 1){
			show_program_semantic(myProgramSemantic->global);
			show_program_semantic_procedures(myProgramSemantic->procs);
		}
	}
	*/
	yyparse();
	return 0;
}

void yyerror(char *s) {
	colcount -= yyleng;

	if(strcmp(yytext, "") == 0){
		colcount++;
	}
	
	printf ("Line %d, col %d: %s: %s\n", linhacount, colcount, s, yytext);
}
