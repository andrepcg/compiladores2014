%{
	int linhacount = 1;
	int colcount = 1;
	int comfim = 0;
	int inicom = 0;
%}

%x	MULTILINECOM
IniCom		"/*"
FimCom		"*/"
SINGLELINECOM	(\/\/)(.)*

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

RESERVED	abstract|assert|break|byte|case|catch|char|continue|default|double|do|enum|extends|final|finally|float|for|implements|import|instanceof|interface|long|native|null|package|private|protected|short|strictfp|super|switch|synchronized|this|throw|throws|transient|try|volatile

ID			([a-zA-Z]|"_"|"$")([a-zA-Z]|"_"|"$"|[0-9])*
INTLIT		(([1-9][0-9]*)|0)|(0[xX][0-9a-fA-F]+)



%%

{IniCom}		{inicom = linhacount; comfim = 0; BEGIN(MULTILINECOM);}
<MULTILINECOM>{FimCom}	{comfim = 1; inicom = 0; BEGIN 0;}
<MULTILINECOM>\n	{linhacount+=1;colcount=1;}
<MULTILINECOM>.		{colcount+=1;}

{SINGLELINECOM}		{linhacount+=1;colcount=yyleng;}

[\n\r]			{linhacount+=1; colcount = 1;}
" "				{colcount+=yyleng;}
\t				{colcount+=yyleng;}

{RESERVED}		{colcount+=yyleng; printf("RESERVED(%s)\n",yytext);}

true|false		{colcount+=yyleng; printf("BOOLLIT(%s)\n",yytext);}
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

{ID}		{colcount+=yyleng; printf("ID(%s)\n",yytext);}
{INTLIT}	{colcount+=yyleng; printf("INTLIT(%s)\n", yytext);}

.			{colcount+=yyleng;printf("Line %d, col %d: illegal character ('%c')\n", linhacount, colcount, yytext[0]);}
%%

int main()
{
	yylex();
	if (inicom != 0 && comfim == 0){
		printf("Line %d, col %d: unterminated comment\n",inicom,colcount);

	}
	return 0;
}

int yywrap()
{
	return 1;
}

