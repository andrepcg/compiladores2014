%{
	int linhacount = 1;
	int colcount = 1;
	int colCom = 0;
	int linhaCom = 0;

%}

%x	MULTILINECOM
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

RESERVED	abstract|continue|for|switch|assert|default|goto|package|synchronized|do|private|this|break|double|implements|protected|throw|byte|import|throws|case|enum|instanceof|transient|catch|extends|short|try|char|final|interface|finally|long|strictfp|volatile|const|float|native|super|null

ID			[a-zA-Z_$][a-zA-Z_$0-9]*
INTLIT		[0-9]+|0x[0-9a-fA-F]+


%%

{BOOLLIT}		{colcount+=yyleng; printf("BOOLLIT(%s)\n",yytext);}
{INT}			{colcount+=yyleng; printf("INT\n");}
{BOOL}			{colcount+=yyleng; printf("BOOL\n");}
{NEW}			{colcount+=yyleng; printf("NEW\n");}
{IF}			{colcount+=yyleng; printf("IF\n");}
{ELSE}			{colcount+=yyleng; printf("ELSE\n");}
{WHILE}			{colcount+=yyleng; printf("WHILE\n");}
{PRINT}			{colcount+=yyleng; printf("PRINT\n");}
{PARSEINT}		{colcount+=yyleng; printf("PARSEINT\n");}
{CLASS}			{colcount+=yyleng; printf("CLASS\n");}
{PUBLIC}		{colcount+=yyleng; printf("PUBLIC\n");}
{STATIC}		{colcount+=yyleng; printf("STATIC\n");}
{VOID}			{colcount+=yyleng; printf("VOID\n");}
{STRING}		{colcount+=yyleng; printf("STRING\n");}
{DOTLENGTH}		{colcount+=yyleng; printf("DOTLENGTH\n");}
{RETURN}		{colcount+=yyleng; printf("RETURN\n");}
{OCURV}			{colcount+=yyleng; printf("OCURV\n");}
{CCURV}			{colcount+=yyleng; printf("CCURV\n");}
{OBRACE}		{colcount+=yyleng; printf("OBRACE\n");}
{CBRACE}		{colcount+=yyleng; printf("CBRACE\n");}
{OSQUARE}		{colcount+=yyleng; printf("OSQUARE\n");}
{CSQUARE}		{colcount+=yyleng; printf("CSQUARE\n");}
{OP1}			{colcount+=yyleng; printf("OP1(%s)\n",yytext);}
{OP2}			{colcount+=yyleng; printf("OP2(%s)\n",yytext);}
{OP3}			{colcount+=yyleng; printf("OP3(%s)\n",yytext);}
{OP4}			{colcount+=yyleng; printf("OP4(%s)\n",yytext);}
{NOT}			{colcount+=yyleng; printf("NOT\n");}
{ASSIGN}		{colcount+=yyleng; printf("ASSIGN\n");}
{SEMIC}			{colcount+=yyleng; printf("SEMIC\n");}
{COMMA}			{colcount+=yyleng; printf("COMMA\n");}

{RESERVED}		{colcount+=yyleng; printf("RESERVED(%s)\n",yytext);}

{ID}			{colcount+=yyleng; printf("ID(%s)\n",yytext);}
{INTLIT}		{colcount+=yyleng; printf("INTLIT(%s)\n", yytext);}

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

int main()
{
	yylex();

	return 0;
}

int yywrap()
{
	return 1;
}
