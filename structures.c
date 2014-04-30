#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

Class* createClass(char *id, DeclList *declaracoes){
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

ParamList* insertFormalParam(Type tipo, char *id, ParamList *lista, int isHead){

	ParamList *novo = (ParamList*) malloc(sizeof(ParamList));
	
	novo->tipo = tipo;
	novo->id = id;
	
	if(!isHead)
		return novo;
		
	ParamList* aux = lista;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = novo;

	return lista;

}


IDList* insertIDList(char *id, IDList *listaIDs){
	IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = listaIDs;
	
	if(listaIDs == NULL)
		return newId;
		
	IDList* aux = listaIDs;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newId;

	return listaIDs;
}

VarDecl* insertVarDecl(Type tipo, char *id, IDList *listaIDs, int iStatic){
	VarDecl *newVar = (VarDecl*) malloc(sizeof(VarDecl));
	newVar->id = id;
	newVar->iStatic = iStatic;
	newVar->idList = listaIDs;
	
	return newVar;

}

VarDeclList* insertVarDeclList(VarDecl *vardecl, VarDeclList *listaDecl){
	VarDeclList *newList = (VarDecl*) malloc(sizeof(VarDecl));
	newList->declaracao = vardecl;
	newList->next = NULL;
	
	if(listaDecl == NULL)
		return newList;
		
	VarDeclList* aux = listaDecl;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newList;

	return listaDecl;
}

VarDecl* setStatic(VarDecl *vardecl, int a){
	vardecl->iStatic = a;
	
	return vardecl;

}

MethodDecl* insertMethodDecl(Type tipo, char *id, ParamList *parametros, VardDeclList *declaracoes, StmtList *stmts){
	MethodDecl *newMethod = (MethodDecl*) malloc(sizeof(MethodDecl));
	
	MethodDecl->type = tipo;
	MethodDecl->id = id;
	MethodDecl->parametros = parametros;
	MethodDecl->declaracoes = declaracoes;
	MethodDecl->stmts = stmts;
	
	return newMethod;
}


