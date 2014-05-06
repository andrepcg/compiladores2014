#ifndef SHOWS_H
#define SHOWS_H

#include "structures.h"
#include "symbol_table.h"


void printClass(Class* class);
void printDeclList(DeclList* list);
void printMethodDecl(MethodDecl *method);
void printMethodBody(MethodDecl *method);
void printMethodStmts(StmtList *stmts, int level);
void printMethodDeclarations(VarDeclList *declaracoes,int level);
void printStatement(Statement *stmt,int level);
void printExpression(Expr *expr, int level);
void ExprTypeToString(OpType type, ExprType op);
void printVarDecl(Type tipo, char *id, int iStatic, int level);
void printMethodParams(ParamList *params, int level);
void print(char *s, int level, int linebreak);
char* idIntFormat(int tipo, char *idLit);
void typeToString(Type type);
void StmtTypeToString(StmtType type);
void printArgs( ArgsList *argsList,int level);
void printIndent(int level);

void printSymbolTables(ClassTable*);
void typeToStringST(Type type, char* dest);
void printMethodTable(ClassTableEntry* table);
void printSymbolTables(ClassTable* table);


#endif
