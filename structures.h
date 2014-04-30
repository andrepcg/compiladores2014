#ifndef STRUCTURES_H
#define STRUCTURES_H

////
// Enums of nodes types.

typedef enum {VARDECL, METHODDECL} DeclType;
typedef enum {VOID_T, INT_T, BOOL_T, INTARRAY, BOOLARRAY, STRINGARRAY, METHOD} Type;
typedef enum {CSTAT, IFELSE, RETURN_T, WHILE_T, PRINT_T, STORE, STOREARRAY} StmtType;
typedef enum {BINOP, UNOP, ID_T, INTLIT_T, BOOLLIT_T, CALL, PARSEINT_T, INDEX, NEWINTARR, NEWBOOLARR} ExprType;
typedef enum {PLUS, MINUS, MUL, DIV, REM, LESSER, GREATER, LEQ, GEQ, DIF, EQ, NOT, DOTLENGTH_T, AND_T, OR_T} OpType;

typedef struct _expr
{
    ExprType type;
    OpType op;
	struct _expr *expr1;
	struct _expr *expr2;
	char *idOrLit;
	ArgsList *argsList;
} Expr;

struct _argsList
{
	Expr *expr;
    struct _argsList *next;
};

/*
	Tipos de statements
	
	{
		statement
		statement
		...
		(stmtList)
	}
	
	ou
	
	if(expr)
		statement
		
	ou
	
	if(expr)
		statement
	else
		statement
		
	ou
		
	while(expr)
		statement

	ou
	
	print(expr);
	
	ou
	
	a = expr;
	
	ou
	
	a[expr] = expr;
	
	ou
	
	return;
	
	ou
	
	return expr;


*/

typedef struct {
	StmtType tipo;
	struct _stmtList *stmts;
	
	Expr *expr1;
	Expr *expr2;
	
	Statement *stmt1;
	Statement *stmt2;
	
} Statement;

typedef struct _stmtList {
	Statement *stmt;
	struct _stmtList *next;
} StmtList;

typedef struct _idList{
	char *id;
	struct _idList *next;
} IDList;

typedef struct {
	Type tipo;
	IDList *idList;
	int iStatic;
} VarDecl;

typedef struct _varDeclList {
	VarDecl *declaracao;
	struct _varDeclList *next;
} VardDeclList;


typedef struct _paramList {
	Type tipo;
	char *id;
	struct _paramList *next;
} ParamList;

typedef struct {
	Type tipo;
	char *id;
	
	ParamList *parametros;
	VardDeclList *declaracoes;
	StmtList *stmts;
	
} MethodDecl;


typedef struct _declList {
	DeclType tipo;
	union{
		VarDecl *varDecl;
		MethodDecl *methodDecl;
	};
    struct _declList *next;
	
} DeclList;

typedef struct {
	char *id;
	DeclList *declaracoes;
} Class;

Class* createClass(char *id, DeclList *declaracoes);
DeclList* insertDecl(DeclType type, void* decl, DeclList* list);
ParamList* insertFormalParam(Type tipo, char *id, ParamList *lista, int isHead);
IDList* insertIDList(char *id, IDList *listaIDs);
VarDecl* insertVarDecl(Type tipo, char *id, IDList *listaIDs, int iStatic);
VarDeclList* insertVarDeclList(VarDecl *vardecl, VarDeclList *listaDecl);


#endif