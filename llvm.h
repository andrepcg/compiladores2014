
#ifndef LLVM_H
#define LLVM_H

#include "structures.h"

void genGlobalVar(VarDecl*);
void genMethod(MethodDecl*);
void genLocalVar(VarDecl*);
void genStmtList(StmtList*);
void genStmt(Statement*);
int buildExpression(Expr*,int,int,char*);
int genExpr(Expr*);
void getExprType(char* llvmType, Expr* expr);
void getTypeLLVM(char*, Type);
void getRetTypeLLVM(char*, Type);

typedef struct _array
{
	char *id;
	char tipo[15];
	int size;
	struct _array *next;

} Array;

Array *getArray(char* id);
void createArray(char *id, char *tipo, int size);

#endif
