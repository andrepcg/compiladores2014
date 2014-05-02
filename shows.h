#ifndef SHOWS_H
#define SHOWS_H

#include "structures.h"


void printClass(Class* class);
void printDeclList(DeclList* list);
void printMethodDecl(MethodDecl *method);
void printMethodBody(MethodDecl *method);
void printMethodStmts(StmtList *stmts, int level);
void printMethodDeclarations(VarDeclList *declaracoes,int level);
void printStatement(Statement *stmt,int level);
void printExpression(Expr *expr, int level);
char* ExprTypeToString(OpType type, ExprType op);
void printVarDecl(Type tipo, char *id, int iStatic, int level);
void printMethodParams(ParamList *params, int level);
void print(char *s, int level, int linebreak);
char* idIntFormat(int tipo, char *idLit);
char* typeToString(Type type);
char* StmtTypeToString(StmtType type);
void printArgs( ArgsList *argsList,int level);


#endif
