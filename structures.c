#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

void createClass(char *id, DeclList *declaracoes){
	Class *class = (Class*) malloc(sizeof(Class));
	class->id = id;
	class->declaracoes = declaracoes;
	
	return class;
}

DeclList* insertDecl(DeclType type, void* decl, DeclList* list)
{
    DeclList* newDecl = (DeclList*) malloc(sizeof(DeclList));
    newDecl->type = type;
    if(type == VARDECL)
        newDecl->varDecl = (VarDecl*) decl;
    else
        newDecl->methodDecl = (MethodDecl*) decl;
    newDecl->next = NULL;

    if(list == NULL)
        return newDecl;

    DeclList* aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newDecl;

    return list;
}

VarDeclList* insertVarDecl(VardDeclList vardecl, Type tipo, char *id, IDList *listaIDs){
	IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = listaIDs;

    VarDecl* newVarDecl = (VarDecl*) malloc(sizeof(VarDecl));
	newVarDecl->type = tipo;
	newVarDecl->isStatic = 0;
	newVarDecl->idList = newId;

    VarDeclList* newVarDeclList = (VarDeclList*) malloc(sizeof(VarDeclList));
    newVarDeclList->varDecl = newVarDecl;
    newVarDeclList->next = NULL;

    if(vardecl==NULL)
        return newVarDeclList;

	VarDeclList* aux = vardecl;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newVarDeclList;

	return vardecl;
	
}

