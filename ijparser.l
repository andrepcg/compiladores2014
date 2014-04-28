%x	MULTILINECOM

%{
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <ctype.h>
	#include "y.tab.h"

	int linhacount = 1;
	int colcount = 1;
	int colCom = 0;
	int linhaCom = 0;
	
	extern int yylval;

%}


IniCom		"/*"
FimCom		"*/"
SINGLELINECOM	"//".*

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

RESERVED	"++"|"--"|abstract|continue|for|switch|assert|default|goto|package|synchronized|do|private|this|break|double|implements|protected|throw|byte|import|throws|case|enum|instanceof|transient|catch|extends|short|try|char|final|interface|finally|long|strictfp|volatile|const|float|native|super|null

ID			[a-zA-Z_$][a-zA-Z_$0-9]*
INTLIT		[0-9]+|0x[0-9a-fA-F]+


%%

{BOOLLIT}		{colcount+=yyleng; return BOOLLIT;}
{INT}			{colcount+=yyleng; return INT;}
{BOOL}			{colcount+=yyleng; return BOOL;}
{NEW}			{colcount+=yyleng; return NEW;}
{IF}			{colcount+=yyleng; return IF;}
{ELSE}			{colcount+=yyleng; return ELSE;}
{WHILE}			{colcount+=yyleng; return WHILE;}
{PRINT}			{colcount+=yyleng; return PRINT;}
{PARSEINT}		{colcount+=yyleng; return PARSEINT;}
{CLASS}			{colcount+=yyleng; return CLASS;}
{PUBLIC}		{colcount+=yyleng; return PUBLIC;}
{STATIC}		{colcount+=yyleng; return STATIC;}
{VOID}			{colcount+=yyleng; return VOID;}
{STRING}		{colcount+=yyleng; return STRING;}
{DOTLENGTH}		{colcount+=yyleng; return DOTLENGTH;}
{RETURN}		{colcount+=yyleng; return RETURN;}
{OCURV}			{colcount+=yyleng; return OCURV;}
{CCURV}			{colcount+=yyleng; return CCURV;}
{OBRACE}		{colcount+=yyleng; return OBRACE;}
{CBRACE}		{colcount+=yyleng; return CBRACE;}
{OSQUARE}		{colcount+=yyleng; return OSQUARE;}
{CSQUARE}		{colcount+=yyleng; return CSQUARE;}
{OP1}			{colcount+=yyleng; return OP1;}
{OP2}			{colcount+=yyleng; return OP2;}
{OP3}			{colcount+=yyleng; return OP3;}
{OP4}			{colcount+=yyleng; return OP4;}
{NOT}			{colcount+=yyleng; return NOT;}
{ASSIGN}		{colcount+=yyleng; return ASSIGN;}
{SEMIC}			{colcount+=yyleng; return SEMIC;}
{COMMA}			{colcount+=yyleng; return COMMA;}
                                   
{RESERVED}		{colcount+=yyleng; return RESERVED;}

{ID}			{colcount+=yyleng; /* yylval.valorID=(char *) strdup(yytext);*/ return ID;}
{INTLIT}		{colcount+=yyleng; /*yylval.valorInteiro=(int) atoi(yytext);*/ return INTLIT;}

{SINGLELINECOM}		{;}

{IniCom}					{colCom = colcount; linhaCom=linhacount; BEGIN MULTILINECOM;}
<MULTILINECOM>.				{colcount +=yyleng; }
<MULTILINECOM>"\n"			{linhacount++;colcount=1;}
<MULTILINECOM><<EOF>>		{printf("Line %d, col %d: unterminated comment\n",linhaCom,colCom); return 0;}
<MULTILINECOM>{FimCom}		{colcount +=2; BEGIN 0;  }

\n							{colcount = 1; linhacount++; }
[ \t\f\r]					{colcount +=yyleng; }
<<EOF>> 					{colcount +=yyleng; yyterminate(); }


.			{printf("Line %d, col %d: illegal character ('%c')\n", linhacount, colcount, yytext[0]); colcount+=yyleng;}
%%

int yywrap()
{
	return 1;
}
