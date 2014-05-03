#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"

#define INDENT 2

void printClass(Class* class){
    printf("Program\n");
    printf("  Id(%s)\n", class->id);

    if(class->declaracoes != NULL)
        printDeclList(class->declaracoes);
    else
        print("NULL", 1, 1);

}

void printDeclList(DeclList* list)
{
    DeclList* aux = list;
    for(; aux != NULL; aux = aux->next){
        if(aux->tipo == VARDECL){

			IDList* auxID = aux->varDecl->idList ;
			printVarDecl(aux->varDecl->tipo, auxID->id, 0, 1);
			
			for(auxID = auxID->next; auxID != NULL; auxID = auxID->next){
				print(idIntFormat(0, auxID->id), 2, 1);
			}
		}
        else if(aux->tipo == METHODDECL)
            printMethodDecl(aux->methodDecl);
    }
}

void printMethodDecl(MethodDecl *method){
	print("MethodDecl", 1, 1);
	
	print(typeToString(method->tipo), 2, 1);
	print(idIntFormat(0, method->id), 2, 1);
	
	printMethodParams(method->parametros, 2);
		
	printMethodBody(method);
	
	
	
}

void printMethodBody(MethodDecl *method){
	print("MethodBody", 2, 1);
	
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
			print(idIntFormat(0, auxID->id), level + 1, 1);
            //printVarDecl(aux->declaracao->tipo, auxID->id, 0, level + 1);
        }

    }

}

void printStatement(Statement *stmt,int level){

	if(stmt == NULL){
		print("Null", level, 1);
		return;
	}
		
    print(StmtTypeToString(stmt->tipo),level,1);
	
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
		print(idIntFormat(0, stmt->id),level + 1, 1);
        printExpression(stmt->expr1, level);

    }
    else if(stmt->tipo==STOREARRAY){
		print(idIntFormat(0, stmt->id),level + 1, 1);
        printExpression(stmt->expr1, level);
        printExpression(stmt->expr2, level);
    }
    
}
void printExpression(Expr *expr, int level){

    if(expr->type == BINOP){
		print(ExprTypeToString(expr->op, BINOP),level + 1,1);
        printExpression(expr->expr1,level + 1);
        printExpression(expr->expr2,level + 1);
    }
    else if(expr->type == UNOP){
        print(ExprTypeToString(expr->op, UNOP),level + 1,1);
		printExpression(expr->expr1, level + 1);
	}

    else if(expr->type == ID_T)
        print(idIntFormat(0, expr->idLit),level+1, 1);

    else if(expr->type == INTLIT_T)
        print(idIntFormat(1, expr->idLit),level+1, 1);
    
    else if(expr->type == BOOLLIT_T)
        print(idIntFormat(2, expr->idLit),level+1, 1);
    
    else if(expr->type == CALL){
		print("Call", level + 1, 1);
        print(idIntFormat(0, expr->idLit),level+2, 1);
        printArgs(expr->argsList,level);
    }
    else if(expr->type == PARSEINT_T){
		print("ParseArgs", level + 1, 1);
        print(idIntFormat(0, expr->idLit),level+2, 1);
        printExpression(expr->expr1,level+1);
    }
    else if(expr->type == INDEX){
		print("LoadArray", level + 1, 1);
        printExpression(expr->expr1,level+1);
        printExpression(expr->expr2,level+1);
    }
    else if(expr->type == NEWINTARR){
		print("NewInt", level + 1, 1);
        printExpression(expr->expr1,level+1);
    }
    else if(expr->type == NEWBOOLARR){
		print("NewBool", level + 1, 1);
        printExpression(expr->expr1,level+1);
    }
}

void printArgs( ArgsList *argsList,int level){
    ArgsList* aux = argsList;
    for(; aux != NULL; aux = aux->next){
        printExpression(aux->expr,level+1);

    }
}

char* ExprTypeToString(OpType type, ExprType op)
{
    char *temp = (char*) malloc(sizeof(char) * 7);
    if(type == PLUS){
		if(op == BINOP)
			temp = "Add";
		else
			temp = "Plus";
			
    }
	else if(type == MINUS){
        if(op == BINOP)
			temp = "Sub";
		else
			temp = "Minus";
			
    }else if(type == MUL)
        temp = "Mul";
    else if(type == DIV)
        temp = "Div";
    else if(type == MOD)
        temp = "Mod";
    else if(type == LESSER)
        temp = "Lt";
    else if(type == GREATER)
        temp = "Gt";
	else if(type == EQ)
        temp = "Eq";
    else if(type == LEQ)
        temp = "Leq";
    else if(type == GEQ)
        temp = "Geq";
    else if(type == DOTLENGTH_T)
        temp = "Length";
    else if(type == AND_T)
        temp = "And";
    else if(type == OR_T)
        temp = "Or";
    else if(type == NOT_)
        temp = "Not";
	else if(type == DIF)
        temp = "Neq";
   

    return temp;
 }

void printVarDecl(Type tipo, char *id, int iStatic, int level){
    print("VarDecl",level, 1);
	//if(iStatic)
		//print("Static", level + 1, 1);
    print(typeToString(tipo), level + 1, 1);
    print(idIntFormat(0, id), level + 1, 1);

}

void printMethodParams(ParamList *params, int level){

	print("MethodParams", level, 1);
	
	ParamList* aux = params;
    for(; aux != NULL; aux = aux->next){
		print("ParamDeclaration", level + 1, 1);
		print(typeToString(aux->tipo), level + 2, 1);
		print(idIntFormat(0, aux->id), level + 2, 1);
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
		
	printf("%s%s%c", spaces, s, lb);
	
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

char* typeToString(Type type)
{
	char *temp = (char*) malloc(sizeof(char) * 18);
    if(type == INT_T)
        temp = "Int";
    else if(type == BOOL_T)
        temp = "Bool";
    else if(type == INTARRAY)
		temp = "IntArray";
    else if(type == BOOLARRAY)
        temp = "BoolArray";
    else if(type == VOID_T)
        temp = "Void";
    else if(type == STRINGARRAY)
        temp = "StringArray";

	return temp;
}

char* StmtTypeToString(StmtType type)
{
	char *temp = (char*) malloc(sizeof(char) * 13);
    if(type == CSTAT)
        temp = "CompoundStat";
    else if(type == IFELSE)
        temp = "IfElse";
    else if(type == RETURN_T)
		temp = "Return";
    else if(type == WHILE_T)
        temp = "While";
    else if(type == PRINT_T)
        temp = "Print";
    else if(type == STORE)
        temp = "Store";
	else if(type == STOREARRAY)
        temp = "StoreArray";

	return temp;
}
