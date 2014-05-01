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
    newDecl->tipo = type;
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

	newVar->iStatic = iStatic;
	newVar->idList = listaIDs;
	
	if(listaIDs == NULL)
		return newVar;
		
	IDList *n = (IDList*) malloc(sizeof(IDList));
	n->id = id;
	n->next = listaIDs;
	
	newVar->idList = n;
	
	return newVar;

}

VarDeclList* insertVarDeclList(VarDecl *vardecl, VarDeclList *listaDecl){
	VarDeclList *newList = (VarDeclList*) malloc(sizeof(VarDeclList));
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

MethodDecl* insertMethodDecl(Type tipo, char *id, ParamList *parametros, VarDeclList *declaracoes, StmtList *stmts){
	MethodDecl *newMethod = (MethodDecl*) malloc(sizeof(MethodDecl));
	
	newMethod->tipo = tipo;
	newMethod->id = id;
	newMethod->parametros = parametros;
	newMethod->declaracoes = declaracoes;
	newMethod->stmts = stmts;
	
	return newMethod;
}

Statement *insertStatement(StmtType tipo,StmtList *stmts,Expr *expr,Expr *expr2,Statement *stmt1,Statement *stmt2){
    Statement *novo = (Statement*) malloc(sizeof(Statement));
    novo->tipo = tipo;
    novo->stmts = stmts;
    novo->expr1 = expr;
    novo->expr2 = expr2;
    novo->stmt1 = stmt1;
    novo->stmt2 = stmt2;

    return novo;


}

StmtList *insertListStatement(Statement *stmt,StmtList *lista){
    StmtList* novo = (StmtList*) malloc(sizeof(StmtList));
    
    novo->stmt=stmt;
    novo->next=lista;

    if(lista==NULL)
        return novo;


    StmtList* aux = lista;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = novo;

    return lista;
  
}


ArgsList* insertArgs(Expr *expr, ArgsList *lista){

	ArgsList *new = (ArgsList*) malloc(sizeof(ArgsList));
	new->expr = expr;
	new->next = NULL;
	
	if(lista == NULL)
		return new;
		
	ArgsList* aux = lista;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = new;

    return lista;
	
}

Expr *insertExpression(ExprType type,OpType op,Expr *expr1,Expr *expr2,ArgsList *argsList){
    Expr *novo = (Expr*) malloc(sizeof(Expr));
    novo->type = type;
    novo->op = op;
    novo->expr1 =  expr1;
    novo->expr2 = expr2;
    novo->argsList = argsList;

    return novo;
}

OpType checkOP(char *op){

    if(strcmp(op,"+")==0)
        return PLUS;
    else if(strcmp(op,"-")==0)
        return MINUS;
    else if(strcmp(op,"&&")==0)
        return AND_T;
    else if(strcmp(op,"||")==0)
        return OR_T;
    else if(strcmp(op,">")==0)
        return GREATER;
    else if(strcmp(op,"<")==0)
        return LESSER;
    else if(strcmp(op,"==")==0)
        return EQ;
    else if(strcmp(op,"!=")==0)
        return DIF;
    else if(strcmp(op,"<=")==0)
        return LEQ;
    else if(strcmp(op,">=")==0)
        return GEQ;
    else if(strcmp(op,"*")==0)
        return MUL;
    else if(strcmp(op,"/")==0)
        return DIV;
    else if(strcmp(op,"%")==0)
        return MOD;
    else if(strcmp(op,"!")==0)
        return NOT;
    else if(strcmp(op,".length")==0)
        return DOTLENGTH_T;

}