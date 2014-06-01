#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {VARDECL, METHODDECL} DeclType;
typedef enum {VOID_T, INT_T, BOOL_T, INTARRAY, BOOLARRAY, STRINGARRAY, METHOD} Type;
typedef enum {CSTAT, IFELSE, RETURN_T, WHILE_T, PRINT_T, STORE, STOREARRAY} StmtType;
typedef enum {BINOP, UNOP, ID_T, INTLIT_T, BOOLLIT_T, CALL, PARSEINT_T, INDEX, NEWINTARR, NEWBOOLARR} ExprType;
typedef enum {PLUS, MINUS, MUL, DIV, MOD, LESSER, GREATER, LEQ, GEQ, DIF, EQ, DOTLENGTH_T, AND_T, OR_T, NOT_} OpType;

typedef struct _argsList ArgsList;

typedef struct _expr
{
    ExprType type;
    OpType op;
	char *idLit;
	struct _expr *expr1;
	struct _expr *expr2;
	ArgsList *argsList;
} Expr;

struct _argsList
{
	Expr *expr;
    struct _argsList *next;
};


typedef struct _stmt{
	StmtType tipo;
	struct _stmtList *stmts;
	char *id;
	Expr *expr1;
	Expr *expr2;
	
	struct _stmt *stmt1;
	struct _stmt *stmt2;
	
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
} VarDeclList;


typedef struct _paramList {
	Type tipo;
	char *id;
	struct _paramList *next;
} ParamList;

typedef struct {
	Type tipo;
	char *id;
	
	ParamList *parametros;
	VarDeclList *declaracoes;
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

typedef struct _class {
	char *id;
	DeclList *declaracoes;
} Class;

Class* criarClasse(char *id, DeclList *declaracoes);
DeclList* inserirDecl(DeclType type, void* decl, DeclList* list);
ParamList* inserirFormalParam(Type tipo, char *id, ParamList *lista, int isHead);
IDList* inserirListaID(char *id, IDList *listaIDs);
VarDecl* inserirDeclVar(Type tipo, char *id, IDList *listaIDs, int iStatic);
VarDecl* setStatic(void *vardecl, int a);
VarDeclList* inserirListaDeclVar(VarDecl *vardecl, VarDeclList *listaDecl);
Expr *inserirExpressao(ExprType type, char *idLit, char *op, Expr *expr1, Expr *expr2, ArgsList *argsList);
OpType checkOP(char *op);
ArgsList* inserirArgs(Expr *expr, ArgsList *lista);
StmtList *inserirListaStatements(Statement *stmt,StmtList *lista);
Statement *inserirStatement(StmtType tipo, char* id, StmtList *stmts,Expr *expr,Expr *expr2,Statement *stmt1,Statement *stmt2);
MethodDecl* inserirDeclMetodo(Type tipo, char *id, ParamList *parametros, VarDeclList *declaracoes, StmtList *stmts);

#endif
