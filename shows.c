#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"

#define INDENT 2

char tmp[40];

void printClass(Class* class){
    printf("Program\n");
    printf("  Id(%s)\n", class->id);

    if(class->declaracoes != NULL)
        printDeclList(class->declaracoes);

}

void printDeclList(DeclList* list)
{
    DeclList* aux = list;
    for(; aux != NULL; aux = aux->next){
        if(aux->tipo == VARDECL){

			IDList* auxID = aux->varDecl->idList ;
			printVarDecl(aux->varDecl->tipo, auxID->id, 0, 1);
			
			for(auxID = auxID->next; auxID != NULL; auxID = auxID->next){
				printIndent(2);
				printf("Id(%s)\n", auxID->id);
			}
		}
        else if(aux->tipo == METHODDECL)
            printMethodDecl(aux->methodDecl);
    }
}

void printMethodDecl(MethodDecl *method){
	printIndent(1);
	printf("MethodDecl\n");
	
	printIndent(2);
	typeToString(method->tipo);
	
	printIndent(2);
	printf("Id(%s)\n", method->id);
	
	printMethodParams(method->parametros, 2);
	printMethodBody(method);
	
	
	
}

void printMethodBody(MethodDecl *method){

	printIndent(2);
	printf("MethodBody\n");
	
	if(method->declaracoes != NULL)
		printMethodDeclarations(method->declaracoes, 3);
	
	if(method->stmts != NULL)
		printMethodStmts(method->stmts, 3);
}

void printMethodStmts(StmtList *stmts, int level){

	StmtList* aux = stmts;
    for(; aux != NULL; aux = aux->next){
		printStatement(aux->stmt,level);
	}

}

void printMethodDeclarations(VarDeclList *declaracoes,int level){
    
    VarDeclList* aux = declaracoes;
    for(; aux != NULL; aux = aux->next){
		
        IDList* auxID = aux->declaracao->idList ;
		printVarDecl(aux->declaracao->tipo, auxID->id, 0, level);
		
        for(auxID = auxID->next; auxID != NULL; auxID = auxID->next){
		
			printIndent(level + 1);
			printf("Id(%s)\n", auxID->id);

        }

    }

}

void printStatement(Statement *stmt,int level){

	printIndent(level);

	if(stmt == NULL){
		printf("Null\n");
		return;
	}
		
    StmtTypeToString(stmt->tipo);
	
    if(stmt->tipo==IFELSE){
        printExpression(stmt->expr1,level);
        printStatement(stmt->stmt1,level + 1);
        printStatement(stmt->stmt2,level + 1);
    }
    else if(stmt->tipo==CSTAT){
        printMethodStmts(stmt->stmts,level + 1);
    }
    else if(stmt->tipo==RETURN_T){
        if(stmt->expr1 != NULL)
            printExpression(stmt->expr1,level);
    }
    else if(stmt->tipo==WHILE_T){
        printExpression(stmt->expr1,level);
		printStatement(stmt->stmt1,level + 1);
    }
    else if(stmt->tipo==PRINT_T){
        printExpression(stmt->expr1,level);
        
    }
    else if(stmt->tipo==STORE){
		printIndent(level + 1);
		printf("Id(%s)\n", stmt->id);

        printExpression(stmt->expr1, level);

    }
    else if(stmt->tipo==STOREARRAY){
		printIndent(level + 1);
		printf("Id(%s)\n", stmt->id);
        printExpression(stmt->expr1, level);
        printExpression(stmt->expr2, level);
    }
    
}
void printExpression(Expr *expr, int level){

	printIndent(level + 1);

    if(expr->type == BINOP){
		ExprTypeToString(expr->op, BINOP);
		printExpression(expr->expr1,level + 1);
        printExpression(expr->expr2,level + 1);
		

    }
    else if(expr->type == UNOP){
		ExprTypeToString(expr->op, UNOP);
		printExpression(expr->expr1, level + 1);
	}

    else if(expr->type == ID_T){
		printf("Id(%s)\n", expr->idLit);
	}

    else if(expr->type == INTLIT_T){
		printf("IntLit(%s)\n", expr->idLit);
	}
    
    else if(expr->type == BOOLLIT_T){
		printf("BoolLit(%s)\n", expr->idLit);
	}
    
    else if(expr->type == CALL){	
		printf("Call\n");
		printIndent(level + 2); printf("Id(%s)\n", expr->idLit);
		printArgs(expr->argsList,level);
    }
    else if(expr->type == PARSEINT_T){
		printf("ParseArgs\n");
        printIndent(level + 2);
		printf("Id(%s)\n", expr->idLit);
        printExpression(expr->expr1,level+1);
    }
    else if(expr->type == INDEX){
		printf("LoadArray\n");
        printExpression(expr->expr1,level+1);
        printExpression(expr->expr2,level+1);
    }
    else if(expr->type == NEWINTARR){
		printf("NewInt\n");
        printExpression(expr->expr1,level+1);
    }
    else if(expr->type == NEWBOOLARR){
		printf("NewBool\n");
        printExpression(expr->expr1,level+1);
    }
}

void printArgs( ArgsList *argsList,int level){
    ArgsList* aux = argsList;
    for(; aux != NULL; aux = aux->next)
        printExpression(aux->expr,level+1);

}

void ExprTypeToString(OpType type, ExprType op){

    if(type == PLUS){
		if(op == BINOP)
			printf("Add\n");
		else
			printf("Plus\n");
			
    }
	else if(type == MINUS){
        if(op == BINOP)
			printf("Sub\n");
		else
			printf("Minus\n");
			
    }else if(type == MUL)
		printf("Mul\n");
		
    else if(type == DIV)
		printf("Div\n");
		
    else if(type == MOD)
		printf("Mod\n");
		
    else if(type == LESSER)
		printf("Lt\n");

    else if(type == GREATER)
		printf("Gt\n");

	else if(type == EQ)
		printf("Eq\n");

    else if(type == LEQ)
		printf("Leq\n");

    else if(type == GEQ)
		printf("Geq\n");

    else if(type == DOTLENGTH_T)
		printf("Length\n");

    else if(type == AND_T)
		printf("And\n");

    else if(type == OR_T)
		printf("Or\n");

    else if(type == NOT_)
		printf("Not\n");

	else if(type == DIF)
		printf("Neq\n");

 }

void printVarDecl(Type tipo, char *id, int iStatic, int level){
	printIndent(level);
    printf("VarDecl\n");
	
	printIndent(level + 1);
	typeToString(tipo);
	
	printIndent(level + 1);
	printf("Id(%s)\n", id);

}

void printMethodParams(ParamList *params, int level){

	printIndent(level);
	printf("MethodParams\n");
	
	ParamList* aux = params;
    for(; aux != NULL; aux = aux->next){
		printIndent(level + 1);
		printf("ParamDeclaration\n");
		
		printIndent(level + 2);
		typeToString(aux->tipo);
		
		printIndent(level + 2);
		printf("Id(%s)\n", aux->id);

	}
}

void printIndent(int level){
	char spaces[16];
	int i;
	for(i = 0; i < INDENT * level; i++){
		printf(" ", spaces);
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
		
	printf("%s%s\n", spaces, s);
	
}

// idOrIntLit = 0 -> Id
char* idIntFormat(int tipo, char *idLit){
	char *tmp = (char*) malloc(sizeof(char) * 25);
	char t[8];
	
	if(tipo == 0)
		sprintf(t, "Id");
	else if(tipo == 1)
		sprintf(t, "IntLit");
	else
		sprintf(t, "BoolLit");
		
	
	sprintf(tmp, "%s(%s)", t, idLit);
	return tmp;
}

void typeToString(Type type)
{
    if(type == INT_T)
		printf("Int\n");
    else if(type == BOOL_T)
		printf("Bool\n");
    else if(type == INTARRAY)
		printf("IntArray\n");
    else if(type == BOOLARRAY)
		printf("BoolArray\n");
    else if(type == VOID_T)
		printf("Void\n");
    else if(type == STRINGARRAY)
		printf("StringArray\n");

}

void StmtTypeToString(StmtType type)
{
    if(type == CSTAT)
		printf("CompoundStat\n");
    else if(type == IFELSE)
		printf("IfElse\n");
    else if(type == RETURN_T)
		printf("Return\n");
    else if(type == WHILE_T)
		printf("While\n");
    else if(type == PRINT_T)
		printf("Print\n");
    else if(type == STORE)
		printf("Store\n");
	else if(type == STOREARRAY)
		printf("StoreArray\n");

}
