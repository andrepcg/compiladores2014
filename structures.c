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
	novo->next = NULL;

	if(isHead){
        novo->next = lista;
        return novo;
    }

    if(lista){
		ParamList* aux = lista;
		for(; aux->next != NULL; aux = aux->next);
		aux->next = novo;

		return lista;
	}
	else 
		return novo;

}


IDList* insertIDList(char *id, IDList *listaIDs){
	IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = NULL;
	
	if(listaIDs == NULL)
		return newId;
		
	IDList* aux = listaIDs;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newId;

	return listaIDs;
}

VarDecl* insertVarDecl(Type tipo, char *id, IDList *listaIDs, int iStatic){
	VarDecl *newVar = (VarDecl*) malloc(sizeof(VarDecl));
	IDList *n = (IDList*) malloc(sizeof(IDList));
	
	newVar->iStatic = iStatic;
	newVar->tipo = tipo;
	
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

VarDecl* setStatic(void *vardecl, int a){
	VarDecl *v = (VarDecl*) vardecl;
	v = (VarDecl*) vardecl;
	v->iStatic = a;
	
	return v;

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

Statement *insertStatement(StmtType tipo, char* id, StmtList *stmts, Expr *expr, Expr *expr2, Statement *stmt1, Statement *stmt2){
    
	if(tipo == CSTAT)
		if(stmts == NULL)
			return NULL;
		else if(stmts->next == NULL)
			return stmts->stmt;
	
	Statement *novo = (Statement*) malloc(sizeof(Statement));
    novo->tipo = tipo;
	novo->id = id;
    novo->stmts = stmts;
    novo->expr1 = expr;
    novo->expr2 = expr2;
    novo->stmt1 = stmt1;
    novo->stmt2 = stmt2;

    return novo;


}

StmtList *insertListStatement(Statement *stmt, StmtList *lista){
    StmtList* novo = (StmtList*) malloc(sizeof(StmtList));
    
    novo->stmt = stmt;
    novo->next = NULL;

    if(lista == NULL)
        return novo;


    StmtList* aux = lista;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = novo;

    return lista;
  
}


ArgsList* insertArgs(Expr *expr, ArgsList *lista, int isHead){

	ArgsList *new = (ArgsList*) malloc(sizeof(ArgsList));
	new->expr = expr;
	new->next = NULL;
	
	if(isHead){
        new->next = lista;
        return new;
    }

    if(lista){
		ArgsList* aux = lista;
		for(; aux->next != NULL; aux = aux->next);
		aux->next = new;

		return lista;
	}
	else 
		return new;
	
}

Expr *insertExpression(ExprType type, char *idLit, char *op, Expr *expr1, Expr *expr2, ArgsList *argsList){
    Expr *novo = (Expr*) malloc(sizeof(Expr));
    novo->type = type;
	novo->idLit = idLit;
    novo->op = (op != NULL) ? checkOP(op) : -1;
    novo->expr1 =  expr1;
    novo->expr2 = expr2;
    novo->argsList = argsList;
	

    return novo; 
}

OpType checkOP(char *op){

    if(strcmp(op, "&&") == 0)
        return AND_T;
    else if(strcmp(op, "||") == 0)
        return OR_T;
    else if(strcmp(op, "<") == 0)
        return LESSER;
    else if(strcmp(op, ">") == 0)
        return GREATER;
    else if(strcmp(op, "<=") == 0)
        return LEQ;
    else if(strcmp(op, ">=") == 0)
        return GEQ;
    else if(strcmp(op, "!=") == 0)
        return DIF;
    else if(strcmp(op, "==") == 0)
        return EQ;
    else if(strcmp(op, "!") == 0)
        return NOT_;
    else if(strcmp(op, "+") == 0)
        return PLUS;
    else if(strcmp(op, "-") == 0)
        return MINUS;
    else if(strcmp(op, "*") == 0)
        return MUL;
    else if(strcmp(op, "/") == 0)
        return DIV;
    else if(strcmp(op, "%") == 0)
        return MOD;
    else if(strcmp(op, ".length") == 0)
        return DOTLENGTH_T;

    return -1;

}
