%{
#include "structures.h"
#include "functions.h"
#include "shows.h"
#include <stdio.h>

extern int linhacount;
extern int colcount;
extern char *yytext;
int yydebug=0;
void yyerror(char *s);
%}

%union{
	is_expression_list* iel;
	is_expression* ie;
	is_infix_expression* iie;
	is_unary_expression* iue;
	int num;
}

%type<iel>statement
%type<ie>expression
%type<iie>infix_expression
%type<iue>unary_expression

%token RESERVED
%token COMMA
%token BOOL INT
%token ID INTLIT
%token IF ELSE WHILE RETURN PRINT
%token OCURV CCURV OBRACE CBRACE OSQUARE CSQUARE OP1 OP2 OP3 OP4 NOT ASSIGN SEMIC


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

/*
BOOLLIT		true|false
INT			int
BOOL		boolean
NEW			new
IF			if
ELSE		else
WHILE		while
PRINT		"System.out.println"
PARSEINT	"Integer.parseInt"
CLASS		class
PUBLIC		public
STATIC		static
VOID		void
STRING		String
DOTLENGTH	".length"
RETURN		return
OCURV		"("
CCURV		")"
OBRACE		"{"
CBRACE		"}"
OSQUARE		"["
CSQUARE		"]"
OP1			("&&"|"||")
OP2			("<"|">"|"=="|"!="|"<="|">=")
OP3			("+"|"-")
OP4			("*"|"/"|"%")
NOT			"!"
ASSIGN		"="
SEMIC		";"
COMMA		","
*/

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
	
	
	// ESTA PARTE EM BAIXO É DO ANO PASSADO
expression
	: 	expression ASSIGN expression	{$$=insert_infix_expression($1, is_ASSIGN, $3);}
	|	expression AND expression		{$$=insert_infix_expression($1, is_AND, $3);}
	|	expression OR expression		{$$=insert_infix_expression($1, is_OR, $3);}
	|	expression EQ expression		{$$=insert_infix_expression($1, is_EQ, $3);}
	|	expression NE expression		{$$=insert_infix_expression($1, is_NE, $3);}
	|	expression LT expression		{$$=insert_infix_expression($1, is_LT, $3);}
	|	expression GT expression		{$$=insert_infix_expression($1, is_GT, $3);}
	|	expression LE expression		{$$=insert_infix_expression($1, is_LE, $3);}
	|	expression GE expression		{$$=insert_infix_expression($1, is_GE, $3);}
	|	expression PLUS expression		{$$=insert_infix_expression($1, is_PLUS, $3);}
	|	expression MINUS expression		{$$=insert_infix_expression($1, is_MINUS, $3);}
	|	expression AST expression		{$$=insert_infix_expression($1, is_AST, $3);}
	|	expression DIV expression		{$$=insert_infix_expression($1, is_DIV, $3);}
	|	expression MOD expression		{$$=insert_infix_expression($1, is_MOD, $3);}
	|	AMP expression
	|	AST expression
	|	PLUS expression
	|	MINUS expression
	|	NOT expression
	|	expression LSQ expression RSQ
	|	ID LPAR RPAR
	|	ID LPAR expression RPAR
	|	ID LPAR expression comma_expression RPAR
	|	PRINTF LPAR expression RPAR
	|	ATOI LPAR expression RPAR
	|	ITOA LPAR expression COMMA expression RPAR
	|	ID
	|	INTLIT
	|	CHRLIT
	|	STRLIT
	|	LPAR expression RPAR
	;

	
%%

int main(){
	yyparse();
	return 0;
}

void yyerror(char *s) {
	printf ("Line %d, col %d: %s: %s\n", linhacount, colcount, s, yytext);
}