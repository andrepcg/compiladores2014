#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"

#define INDENT 2

void printClass(Class* class){
    printf("Program\n");
    printf("  Id(%s)\n", class->id);

    if(class->declaracoes != NULL)
        printDeclList(class->declaracoes);
    else
        printf("  NULL\n");

}

void printDeclList(DeclList* list)
{
    DeclList* aux = list;
    for(; aux != NULL; aux = aux->next){
        if(aux->tipo == VARDECL)
            //printFieldDecl(aux->varDecl);
			return;
        else if(aux->tipo == METHODDECL)
            printMethodDecl(aux->methodDecl);
    }
}

void printMethodDecl(MethodDecl *method){
	print("MethodDecl", 1, 1);
	
	//print(idFormat(typeToString(method->tipo)), 2, 1);
	print(typeToString(method->tipo), 2, 1);
	print(idFormat(method->id), 2, 1);
	
	if(method->parametros != NULL)
		printMethodParams(method->parametros, 2);
	
}

void printMethodParams(ParamList *params, int level){

	print("MethodParams", level, 1);
	
	ParamList* aux = params;
    for(; aux != NULL; aux = aux->next){
		print("ParamDeclaration", level + 1, 1);
		print(typeToString(aux->tipo), level + 2, 1);
		print(idFormat(aux->id), level + 2, 1);
	}
}

void print(char *s, int level, int linebreak){
	char spaces[16];
	char lb = (linebreak == 1) ? '\n' : '\0';
	int i;
	
	for(i = 0; i < INDENT * level; i++){
		spaces[i] = ' ';
		if(i == INDENT * level - 1)
            spaces[i + 1] = '\0';
	}
		
	printf("%s%s%c", spaces, s, lb);
	
}

char* idFormat(char *id){
	char *tmp = (char*) malloc(sizeof(char) * 25);
	sprintf(tmp, "Id(%s)", id);
	return tmp;
}

char* typeToString(Type type)
{
	char *temp = (char*) malloc(sizeof(char) * 18);
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