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

Class *programa;

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
	:	CLASS ID OBRACE CBRACE						{$$ = createClass($2, NULL); programa = $$;}
	|	CLASS ID OBRACE program_multi CBRACE		{$$ = createClass($2, $4); programa = $$;}
	;

	
program_multi
	:	field_decl									{$$=insertDecl(VARDECL, $1, NULL);}
	|	method_decl									{$$=insertDecl(METHODDECL, $1, NULL);}
	|	program_multi field_decl					{$$=insertDecl(VARDECL, $2, $1);}
	|	program_multi method_decl					{$$=insertDecl(METHODDECL, $2, $1);}
	;

field_decl
	:	STATIC var_decl								{$$ = setStatic($2,1);}
	;
	
method_decl
	:	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE CBRACE										{$$ = insertMethodDecl($3, $4, $6, NULL, NULL);}
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE multi_var_decl statement_multi CBRACE		{$$ = insertMethodDecl($3, $4, $6, $9, $10);}
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE multi_var_decl CBRACE						{$$ = insertMethodDecl($3, $4, $6, $9, NULL);}
	|	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE statement_multi CBRACE						{$$ = insertMethodDecl($3, $4, $6, $9, NULL);}
	;
	
type_void
	:	type		{$$ = $1;}
	|	VOID		{$$ = VOID_T;}
	;

formal_params
	:	type ID comma_type_id_multi				{$$ = insertFormalParam($1, $2, $3, 1);}
	|	STRING OSQUARE CSQUARE ID				{$$ = insertFormalParam(STRINGARRAY, $4, NULL, 1);}
	|											{$$ = NULL;}
	;
	
comma_type_id_multi
	:	comma_type_id_multi COMMA type ID		{$$ = insertFormalParam($3, $4, $1, 0);}
	|											{$$ = NULL;}
	;
	
type
	:	INT										{$$ = INT_T;}
	|	BOOL									{$$ = BOOL_T;}
	|	INT OSQUARE CSQUARE						{$$ = INTARRAY;}
	|	BOOL OSQUARE CSQUARE					{$$ = BOOLRRAY;}
	;
	
multi_var_decl
	:	var_decl								{$$ = $1}
	|	multi_var_decl var_decl					{$$ = insertVarDeclList($2, $1)}
	;
	
var_decl
	:	type ID comma_id_multi SEMIC		{$$ = insertVarDecl($1, $2, $3, 0)}
	;

comma_id_multi
	:	comma_id_multi COMMA ID				{$$ = insertIDList($3, $1);}
	|										{$$ = NULL;}
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
	:	NEW INT OSQUARE expr CSQUARE      
	|	NEW BOOL OSQUARE expr CSQUARE      
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
