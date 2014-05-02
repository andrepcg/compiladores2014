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
	Type type;
	ExprType exprtype;

	struct _class *class;
	DeclList *decllist;
	VarDecl *vardecl;
	MethodDecl *methoddecl;
	ParamList *paramlist;
	VarDeclList *vardecllist;
	IDList *idlist;	
	StmtList *stmtlist;
	Statement *stmt;	
	Expr *expr;
	ArgsList *argslist;
}


%token <token> RESERVED COMMA BOOL INT ID INTLIT IF ELSE WHILE RETURN PRINT BOOLLIT NEW PARSEINT PUBLIC STATIC VOID CLASS OCURV CCURV OBRACE CBRACE OSQUARE CSQUARE OP1 OP2 OP3 OP4 NOT ASSIGN SEMIC STRING DOTLENGTH

%type <class>		program
%type <decllist>	program_multi
%type <vardecl>		var_decl field_decl
%type <methoddecl>	method_decl
%type <paramlist>	formal_params comma_type_id_multi
%type <vardecllist>	multi_var_decl 
%type <idlist>		comma_id_multi
%type <stmtlist>	statement_multi
%type <stmt>		statement
%type <expr>		expr exprindex exprnotindex
%type <argslist> 	args comma_expr_multi
%type <type>		type_void type

%nonassoc EXPR1REDUCE
%nonassoc IF 
%nonassoc ELSE

%left OP1
%left OP2
%left OP3
%left OP4
%right ASSIGN
%left OSQUARE
%left OBRACE
%left NOT
%left DOTLENGTH
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
	:	PUBLIC STATIC type_void ID OCURV formal_params CCURV OBRACE multi_var_decl statement_multi CBRACE		{$$ = insertMethodDecl($3, $4, $6, $9, $10);}
	;
	
type_void
	:	type		{$$ = $1;}
	|	VOID		{$$ = VOID_T;}
	;

formal_params
	:	type ID comma_type_id_multi				{$$ = insertFormalParam($1, $2, $3);}
	|	STRING OSQUARE CSQUARE ID				{$$ = insertFormalParam(STRINGARRAY, $4, NULL);}
	|											{$$ = NULL;}
	;
	
comma_type_id_multi
	:	comma_type_id_multi COMMA type ID		{$$ = insertFormalParam($3, $4, $1);}
	|											{$$ = NULL;}
	;
	
type
	:	INT										{$$ = INT_T;}
	|	BOOL									{$$ = BOOL_T;}
	|	INT OSQUARE CSQUARE						{$$ = INTARRAY;}
	|	BOOL OSQUARE CSQUARE					{$$ = BOOLARRAY;}
	;
	
multi_var_decl
	:	multi_var_decl var_decl					{$$ = insertVarDeclList($2, $1);}
	|											{$$ = NULL;}
	;
	
var_decl
	:	type ID comma_id_multi SEMIC		{$$ = insertVarDecl($1, $2, $3, 0);}
	;

comma_id_multi
	:	comma_id_multi COMMA ID				{$$ = insertIDList($3, $1);}
	|										{$$ = NULL;}
	;
	
statement
    :   OBRACE statement_multi CBRACE				    {$$=insertStatement(CSTAT,NULL,$2,NULL,NULL,NULL,NULL);}
	|   IF OCURV expr CCURV statement ELSE statement  %prec ELSE   {$$=insertStatement(IFELSE,NULL,NULL,$3,NULL,$5,$7);}    
	|   IF OCURV expr CCURV statement     	%prec IF	    	{$$=insertStatement(IFELSE,NULL,NULL,$3,NULL,$5,NULL);}
    |   WHILE OCURV expr CCURV statement                {$$=insertStatement(WHILE_T,NULL,NULL,$3,NULL,$5,NULL);}
    |   PRINT OCURV expr CCURV SEMIC                    {$$=insertStatement(PRINT_T,NULL,NULL,$3,NULL,NULL,NULL);}
    |   ID ASSIGN expr SEMIC                            {$$=insertStatement(STORE,$1,NULL,$3,NULL,NULL,NULL);}
    |   ID OSQUARE expr CSQUARE ASSIGN expr SEMIC       {$$=insertStatement(STOREARRAY,$1,NULL,$3,$6,NULL,NULL);}
    |   RETURN SEMIC                                    {$$=insertStatement(RETURN_T,NULL,NULL,NULL,NULL,NULL,NULL);}
    |   RETURN expr SEMIC                               {$$=insertStatement(RETURN_T,NULL,NULL,$2,NULL,NULL,NULL);}
    ;
    
statement_multi     
    :   statement_multi statement   {$$ = insertListStatement($2,$1);}
    |                               {$$ = NULL;}  
    ;
	
expr
    :   exprindex   %prec EXPR1REDUCE                   {$$=$1;}                  
    |   exprindex OSQUARE expr CSQUARE                  {$$=insertExpression(INDEX,NULL, NULL,$1,$3,NULL);}                        
    |   exprnotindex                                    {$$=$1;}
	;
    
    
exprindex
    :   expr OP1 expr                                   {$$=insertExpression(BINOP, NULL,$2,$1,$3,NULL);}
    |   expr OP2 expr                                   {$$=insertExpression(BINOP, NULL,$2,$1,$3,NULL);}
    |   expr OP3 expr                                   {$$=insertExpression(BINOP, NULL,$2,$1,$3,NULL);}
    |   expr OP4 expr                                   {$$=insertExpression(BINOP, NULL,$2,$1,$3,NULL);}
    |	ID												{$$=insertExpression(ID_T, $1, NULL,NULL,NULL,NULL);}
	|	INTLIT											{$$=insertExpression(INTLIT_T, $1, NULL,NULL,NULL,NULL);}
	|	BOOLLIT											{$$=insertExpression(BOOLLIT_T, $1, NULL,NULL,NULL,NULL);}
	|   OCURV expr CCURV                                {$$=$2;}
    |   expr DOTLENGTH                                  {$$=insertExpression(UNOP, NULL,$2,$1,NULL,NULL);}
    |   NOT expr          %prec UNARY                   {$$=insertExpression(UNOP, NULL,$1,$2,NULL,NULL);}
    |   OP3 expr                                        {$$=insertExpression(UNOP, NULL,$1,$2,NULL,NULL);}
    |   PARSEINT OCURV ID OSQUARE expr CSQUARE CCURV    {$$=insertExpression(PARSEINT_T, $3,NULL,$5,NULL,NULL);}
    |   ID OCURV CCURV                                  {$$=insertExpression(CALL, NULL,NULL,NULL,NULL,NULL);}
    |   ID OCURV args CCURV                             {$$=insertExpression(CALL, NULL,NULL,NULL,NULL,$3);}
    ;

exprnotindex
    :   NEW INT OSQUARE expr CSQUARE                    {$$=insertExpression(NEWINTARR,NULL,NULL,$4,NULL,NULL);}
    |   NEW BOOL OSQUARE expr CSQUARE                   {$$=insertExpression(NEWBOOLARR,NULL,NULL,$4,NULL,NULL);}
    ;

args
	:	expr comma_expr_multi			{$$ = insertArgs($1, $2);}
	;
	
comma_expr_multi
	:	comma_expr_multi COMMA expr		{$$ = insertArgs($3, $1);}
	|									{$$ = NULL;}
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
	
	
	if(printTree)
		printClass(programa);
		/*
	if(printSymbols)
		printSymbolTables(symbolsTable);*/

	//freeProgram(myProgram, symbolsTable);
    return 0;
}

void yyerror(char *s) {
	printf("Line %d, col %d: %s: %s\n", prevLineNo, prevColNo, s, yytext);
}
