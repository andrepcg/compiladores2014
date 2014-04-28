%x	MULTILINECOM

%{
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <ctype.h>
	#include "y.tab.h"
	
	#define INITCOL  1
	#define INITLINE 1
	
	void colCount(int l);

	int colNo = INITCOL;
	int prevColNo = INITCOL;
	int lineNo = INITLINE;
	int prevLineNo = INITLINE;
	int lineScom = 0, colScom = 0;
	
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

{BOOLLIT}		{colCount(yyleng); return BOOLLIT;}
{INT}			{colCount(yyleng); return INT;}
{BOOL}			{colCount(yyleng); return BOOL;}
{NEW}			{colCount(yyleng); return NEW;}
{IF}			{colCount(yyleng); return IF;}
{ELSE}			{colCount(yyleng); return ELSE;}
{WHILE}			{colCount(yyleng); return WHILE;}
{PRINT}			{colCount(yyleng); return PRINT;}
{PARSEINT}		{colCount(yyleng); return PARSEINT;}
{CLASS}			{colCount(yyleng); return CLASS;}
{PUBLIC}		{colCount(yyleng); return PUBLIC;}
{STATIC}		{colCount(yyleng); return STATIC;}
{VOID}			{colCount(yyleng); return VOID;}
{STRING}		{colCount(yyleng); return STRING;}
{DOTLENGTH}		{colCount(yyleng); return DOTLENGTH;}
{RETURN}		{colCount(yyleng); return RETURN;}
{OCURV}			{colCount(yyleng); return OCURV;}
{CCURV}			{colCount(yyleng); return CCURV;}
{OBRACE}		{colCount(yyleng); return OBRACE;}
{CBRACE}		{colCount(yyleng); return CBRACE;}
{OSQUARE}		{colCount(yyleng); return OSQUARE;}
{CSQUARE}		{colCount(yyleng); return CSQUARE;}
{OP1}			{colCount(yyleng); return OP1;}
{OP2}			{colCount(yyleng); return OP2;}
{OP3}			{colCount(yyleng); return OP3;}
{OP4}			{colCount(yyleng); return OP4;}
{NOT}			{colCount(yyleng); return NOT;}
{ASSIGN}		{colCount(yyleng); return ASSIGN;}
{SEMIC}			{colCount(yyleng); return SEMIC;}
{COMMA}			{colCount(yyleng); return COMMA;}
                                   
{RESERVED}		{colCount(yyleng); return RESERVED;}

{ID}			{colCount(yyleng); /* yylval.valorID=(char *) strdup(yytext);*/ return ID;}
{INTLIT}		{colCount(yyleng); /*yylval.valorInteiro=(int) atoi(yytext);*/ return INTLIT;}

{SINGLELINECOM}		{prevColNo = colNo; colNo = INITCOL;}

{IniCom}					{BEGIN MULTILINECOM; lineScom = lineNo; colScom = colNo; colCount(yyleng);}
<MULTILINECOM>.				{colCount(yyleng);}
<MULTILINECOM>"\n"			{prevLineNo = lineNo++; prevColNo = colNo; colNo = INITCOL;}
<MULTILINECOM><<EOF>>		{colCount(0); printf("Line %d, col %d: unterminated comment\n",lineScom, colScom); return 0;}
<MULTILINECOM>{FimCom}		{colCount(yyleng); BEGIN 0;  }

\n							{prevLineNo = lineNo++; prevColNo = colNo; colNo = INITCOL;}
[ \t\f\r]					{colCount(yyleng);}
<<EOF>> 					{colCount(0); return yytext[0];}


.			{printf("Line %d, col %d: illegal character ('%s')\n", lineNo, colNo, yytext); colNo++;}
%%

void colCount(int l){
	prevLineNo = lineNo;
	prevColNo = colNo;
	colNo += l;
}

int yywrap()
{
	return 1;
}
