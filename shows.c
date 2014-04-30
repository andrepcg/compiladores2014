#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"

#define INDENT 2

void printClass(Class* class){
    printf("Program\n");
    printf("\tId(%s)\n", class->id);

    if(class->declList != NULL)
        printDeclList(class->declList);
    else
        printf("\tNULL\n");
}

void printDeclList(DeclList* list)
{
    DeclList* aux = list;
    for(; aux != NULL; aux = aux->next){
        if(aux->type == VARDECL)
            //printFieldDecl(aux->varDecl);
			return;
        else if(aux->type == METHODDECL)
            printMethodDecl(aux->methodDecl);
    }
}

void printMethodDecl(MethodDecl *method){
	print("MethodDecl", 1, 1);
	
	print(idFormat(typeToString(method->tipo)), 2, 1);
	
	if(method->parametros != NULL)
		printMethodParams(method->parametros, 2);
	
}

void printMethodParams(ParamList *params, int level){

	print("MethodParams", level, 1);
	
	ParamList* aux = params;
    for(; aux != NULL; aux = aux->next){
		print("ParamDeclaration", level + 1, 1);
		print(typeToString(params->tipo), level + 2, 1);
		print(idFormat(params->id), level + 2, 1);
	}
}

void print(char *s, int level, int linebreak){
	char spaces[16];
	char lb = (linebreak == 1) ? '\n' : '';
	int i;
	
	for(i = 0; i < INDENT * level; i++)
		spaces[i] = " ";
		
	printf("%s%s%s", spaces, s, lb);
	
}

char* idFormat(char *id){
	char tmp[25];
	sprintf(tmp, "Id(%s)", id);
	return tmp;
}

char* typeToString(Type type)
{
	char temp[18];
    if(type == INT_T)
        temp = "Int";
    else if(type == BOOL_T)
        temp = "Bool";
    else if(type == INTARRAY)
		temp = "IntArray";
    else if(type == BOOLARRAY)
        temp = "BoolArray";
    else if(type == VOID_T)
        temp = "Void";
    else if(type == STRINGARRAY)
        temp = "StringArray";

	return temp;
}