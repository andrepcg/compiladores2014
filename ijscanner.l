%{
int col=1;
int line=1;
int comm_col=0;
int comm_line=0;
%}
%X COMMENT
%%
"true"|"false"				{ col += yyleng; printf("BOOLLIT(%s)\n",yytext);}
"int"						{ col += yyleng; printf("INT\n");}
"boolean"					{ col += yyleng; printf("BOOL\n");}
"new"						{ col += yyleng; printf("NEW\n");}
"if"						{ col += yyleng; printf("IF\n");}
"else"						{ col += yyleng; printf("ELSE\n");}	
"while"						{ col += yyleng; printf("WHILE\n");}
"System.out.println"		{ col += yyleng; printf("PRINT\n");}
"Integer.parseInt"			{ col += yyleng; printf("PARSEINT\n");}
"class"						{ col += yyleng; printf("CLASS\n");}
"public"					{ col += yyleng; printf("PUBLIC\n");}
"static"					{ col += yyleng; printf("STATIC\n");}
"void"						{ col += yyleng; printf("VOID\n");}
"String"					{ col += yyleng; printf("STRING\n");}
".length"					{ col += yyleng; printf("DOTLENGTH\n");}
"return"					{ col += yyleng; printf("RETURN\n");}
"("							{ col += yyleng; printf("OCURV\n");}
")"							{ col += yyleng; printf("CCURV\n");}
"{"							{ col += yyleng; printf("OBRACE\n");}
"}"							{ col += yyleng; printf("CBRACE\n");}
"["							{ col += yyleng; printf("OSQUARE\n");}
"]"							{ col += yyleng; printf("CSQUARE\n");}
"&&"|"||"					{ col += yyleng; printf("OP1(%s)\n",yytext);}
"<"|">"|"=="|"!="|"<="|">="	{ col += yyleng; printf("OP2(%s)\n",yytext);}
"+"|"-"						{ col += yyleng; printf("OP3(%s)\n",yytext);}
"*"|"/"|"%"					{ col += yyleng; printf("OP4(%s)\n",yytext);}
"!"							{ col += yyleng; printf("NOT\n");}
"="							{ col += yyleng; printf("ASSIGN\n");}
";"							{ col += yyleng; printf("SEMIC\n");}
","							{ col += yyleng; printf("COMMA\n");}
"abstract"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"continue"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }	
"for"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"switch"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"assert"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"default"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"goto"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"package"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"synchronized"				{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"do"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"private"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"this"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"break"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"double"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"implements"				{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"protected"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"throw"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"byte"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"import"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"throws"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"case"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"enum"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"instanceof"				{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"transient"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"catch"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"extends"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"short"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"try"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"char"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"final"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"interface"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"finally"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"long"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"strictfp"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"volatile"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"const"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"float"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"native"					{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"super"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
"null"						{ col += yyleng; printf("RESERVED(%s)\n",yytext); }
[a-zA-Z_$][a-zA-Z_$0-9]*			{ col += yyleng; printf("ID(%s)\n",yytext); }
0x[0-9a-fA-F]+|	[0-9]+				{ col += yyleng; printf("INTLIT(%s)\n",yytext); }
"//".*						{ ;} 
"/*"						{ BEGIN COMMENT; comm_col = col; comm_line=line;}
<COMMENT>.					{ col +=yyleng; }
<COMMENT>"\n"					{ line++;col=1;}
<COMMENT><<EOF>>				{ printf("Line %d, col %d: unterminated comment\n",comm_line,comm_col); return 0;}
<COMMENT>"*/"					{ BEGIN 0; col +=2; }

"\n"						{ col =1; line+=1; }
[ \t\f\r]					{ col +=yyleng; }
<<EOF>> 					{ col +=yyleng; yyterminate(); }

.							{printf("Line %d, col %d: illegal character('%c')\n", line, col, yytext[0]); col+=yyleng; }
%%
int main()
{
yylex();
}

int yywrap()
{
return 1;
}
