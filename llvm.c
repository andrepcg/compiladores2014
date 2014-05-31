#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm.h"
#include "symbol_table.h"

#define MAX_LLVM_TYPE_SIZE 15

MethodTable* currentLocalTable;

Type curFunctionType;
int varNumber, ifNumber, whileNumber, indexNumber;


char* llvmTypes[6] = {"void", "i32", "i1", "i32*", "i1*", "i8**"};
char* OpTypeLLVM[11] = {"add", "sub", "mul", "sdiv", "srem", "icmp slt", "icmp sgt", "icmp sle", "icmp sge", "icmp ne", "icmp eq"};
char* llvmRetTypes[6] = {"void", "i32", "i1", "{i32, i32*}", "{i32, i1*}", "i8**"};

char *nome;
int returned = 0;
Array *arraysList;

void generateLLVM(Class* programa)
{
	printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*) nounwind readonly\n\n");
    printf("@.str0 = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
	printf("@str.false = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n");
	printf("@str.true = private unnamed_addr constant [7 x i8] c\"true\\0A\\00\\00\"\n\n");

    DeclList* aux = programa->declaracoes;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->tipo == VARDECL)
            genGlobalVar(aux->varDecl);
        else if(aux->tipo == METHODDECL)
            genMethod(aux->methodDecl);
    }
}

void genGlobalVar(VarDecl* varDecl)
{
    char initVal[5];
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "@%s = global %s %s\n";
    const char llvmOutArr[] = "@.%s = global i32 0\n";
    const char llvmOutInitLenght[] = "\tstore i32 0, i32* @.%s\n";

    getTypeLLVM(llvmType, varDecl->tipo);

    if(varDecl->tipo == INT_T || varDecl->tipo == BOOL_T)
        sprintf(initVal, "0");
    else
        sprintf(initVal, "null");

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
    {
        if(varDecl->tipo != INT_T && varDecl->tipo != BOOL_T)
            printf(llvmOutArr, aux->id);
        printf(llvmOut, aux->id, llvmType, initVal);
    }

    printf("\n");
}

void genMethod(MethodDecl* methodDecl)
{
    curFunctionType = methodDecl->tipo;
    varNumber = ifNumber = whileNumber = indexNumber = 1;
    currentLocalTable = getLocalTable(methodDecl->id);

    char llvmType[MAX_LLVM_TYPE_SIZE];
    getRetTypeLLVM(llvmType, methodDecl->tipo);

    printf("define %s @%s(", llvmType, methodDecl->id);

    ParamList* aux = methodDecl->parametros;
    if(aux != NULL)
    {
        getTypeLLVM(llvmType, aux->tipo);
        if(aux->tipo == INTARRAY || aux->tipo == BOOLARRAY || aux->tipo == STRINGARRAY)
            printf("i32 %%.%s, ", aux->id);
        printf("%s %%%s", llvmType, aux->id);
        aux = aux->next;
    }
    for(; aux != NULL; aux = aux->next)
    {
        getTypeLLVM(llvmType, aux->tipo);
        if(aux->tipo == INTARRAY || aux->tipo == BOOLARRAY || aux->tipo == STRINGARRAY)
            printf(", i32 %%.%s", aux->id);
        printf(", %s %%%s", llvmType, aux->id);
    }

    printf(")\n{\n");

    VarDeclList* aux2 = methodDecl->declaracoes;
    for(; aux2 != NULL; aux2 = aux2->next)
        genLocalVar(aux2->declaracao);

    genStmtList(methodDecl->stmts);
	
	if(!returned)
		printf("\tret void\n");


    printf("}\n\n");
	
	returned = 0;
}

void genLocalVar(VarDecl* varDecl)
{
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "\t%%%s = alloca %s\n";
    const char llvmOutArr[] = "\t%%.%s = alloca i32\n";
    const char llvmOutInitLenght[] = "\tstore i32 0, i32* %%.%s\n";

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
    {
        if(varDecl->tipo == INT_T || varDecl->tipo == BOOL_T){
			printf(llvmOut, aux->id, llvmTypes[varDecl->tipo]);
        }
		else{
		/*
			printf(llvmOutArr, aux->id);
            printf(llvmOutInitLenght, aux->id);
			*/
		}
    }
}

void genStmtList(StmtList* list)
{
    StmtList* aux3 = list;
    for(; aux3 != NULL; aux3 = aux3->next)
        genStmt(aux3->stmt);
}

void genStmt(Statement* stmt)
{
    if(stmt->tipo == CSTAT)
        genStmtList(stmt->stmts);
    else if(stmt->tipo == IFELSE)
    {
        int compVarNumber = genExpr(stmt->expr1);
        printf("\tbr i1 %%%d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber, ifNumber, ifNumber);

        printf("ifthen%d:\n", ifNumber);
        if(stmt->stmt1 != NULL)
            genStmt(stmt->stmt1);
        printf("\tbr label %%ifend%d\n\n", ifNumber);

        printf("ifelse%d:\n", ifNumber);
        if(stmt->stmt2 != NULL)
            genStmt(stmt->stmt2);
        printf("\tbr label %%ifend%d\n\n", ifNumber);

        printf("ifend%d:\n", ifNumber);

        ifNumber++;
    }
    else if(stmt->tipo == RETURN_T)
    {
        if(stmt->expr1 != NULL)
        {
            char llvmType[MAX_LLVM_TYPE_SIZE];
            int exprVarNumber = genExpr(stmt->expr1);
			char varType;
			
            if(curFunctionType == INT_T || curFunctionType == BOOL_T){
				if(stmt->expr1->type == INTLIT_T || stmt->expr1->type == BOOLLIT_T )
					varType = '\0';
				else
					varType = '%';
					
                printf("\tret %s %c%d\n", llvmRetTypes[curFunctionType], varType, exprVarNumber);
				
            }
			else
                printf("\tret i32 %%.%d, %s %%%d\n", exprVarNumber, llvmRetTypes[curFunctionType], exprVarNumber);
            
        }
        else
            printf("\t ret void\n");
			
		returned = 1;
    }
    else if(stmt->tipo == WHILE_T)
    {
        printf("whilestart%d:\n", whileNumber);
        int exprVarNumber = genExpr(stmt->expr1);
        printf("\tbr i1 %%%d, label %%whiledo%d, label %%whileend%d\n\n", exprVarNumber, whileNumber, whileNumber);

        printf("whiledo%d:\n", whileNumber);
        genStmt(stmt->stmt1);

        printf("\nwhileend%d:\n", whileNumber);

        whileNumber++;
    }
    else if(stmt->tipo == PRINT_T){

		int ret = genExpr(stmt->expr1);

		Type t;
		if(stmt->expr1->type == ID_T){
			t = getSymbolFromLocalOrGlobal(stmt->expr1->idLit);
		}
		
		
		if(t == BOOL_T || (stmt->expr1->expr1 != NULL && (strcmp(stmt->expr1->expr1->idLit, "true") == 0 || strcmp(stmt->expr1->expr1->idLit, "false") == 0))){
			printf("\t%%%d = zext i1 %%%d to i32\n", varNumber++, ret);
			printf("\t%%%d = getelementptr inbounds [2 x i8*]* @%s, i32 0, i32 %%%d\n", varNumber++, stmt->id, varNumber-1);
			printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber-1);
			printf("\tcall i32 (i8*, ...)* @printf(i8* %%%d)\n", varNumber-1);

		}
		else
			printf("\tcall i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str2, i32 0, i32 0), i32 %%%d)\n", ret);

	}
    else if(stmt->tipo == STORE)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char exprType[20];
        char llvmType[MAX_LLVM_TYPE_SIZE];
        if(stmt->expr1->type==NEWINTARR || stmt->expr1->type==NEWBOOLARR){
            nome=stmt->id;

        }
        int exprVarNumber = genExpr(stmt->expr1);

        Type varType = getSymbolFromLocal(stmt->id);
      
        if(varType == -1){
            isLocal = 0;
            varType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        getExprType(exprType,stmt->expr1);
        if(stmt->expr1->type!=NEWINTARR && stmt->expr1->type!=NEWBOOLARR){

            if(stmt->expr1->type==INTLIT_T ||stmt->expr1->type==BOOLLIT_T )
                printf("\tstore %s %d, %s* %s%s\n", exprType, exprVarNumber, llvmTypes[varType], varDeclSymbol, stmt->id);
            else
                printf("\tstore %s %%%d, %s* %s%s\n", exprType, exprVarNumber, llvmTypes[varType], varDeclSymbol, stmt->id);
        }
    }
    else if(stmt->tipo == STOREARRAY)
    {
        int isLocal = 1;
        char varDeclSymbol[5];

        char llvmType2[MAX_LLVM_TYPE_SIZE];
        int indexVarNumber = genExpr(stmt->expr1);
        int exprVarNumber = genExpr(stmt->expr2);

        Type arrayType = getSymbolFromLocal(stmt->id);
        if(arrayType == -1)
        {
            isLocal = 0;
            arrayType = getSymbolFromGlobal(stmt->id);
        }
		
		Array *var = getArray(stmt->id);

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        if(stmt->expr1->type==INTLIT_T ||stmt->expr1->type==BOOLLIT_T ){
			if(var != NULL)
				printf("\t%%ind%d = getelementptr [%d x %s]* %s%s, i32 0, i32 %d\n", indexNumber, var->size, var->tipo, varDeclSymbol, stmt->id, indexVarNumber);
			else
				printf("\t%%ind%d = getelementptr %s %s%s, i32 %d\n", indexNumber, llvmTypes[arrayType], varDeclSymbol, stmt->id, indexVarNumber);
        
		}
		else
            printf("\t%%ind%d = getelementptr %s %s%s, i32 %%%d\n", indexNumber, llvmTypes[arrayType], varDeclSymbol, stmt->id, indexVarNumber);
       
		if(stmt->expr2->type==INTLIT_T ||stmt->expr2->type==BOOLLIT_T )
			printf("\tstore %s %d, %s %%ind%d\n", llvmTypes[arrayType - 2], exprVarNumber, llvmTypes[arrayType], indexNumber);
        else
			printf("\tstore %s %%%d, %s %%ind%d\n", llvmTypes[arrayType - 2], exprVarNumber, llvmTypes[arrayType], indexNumber);
		indexNumber++;
    }
	
}

void getExprType(char* llvmType, Expr* expr){
    if(expr->type==INTLIT_T)
        sprintf(llvmType,"i32");
    else if(expr->type==BOOLLIT_T)
        sprintf(llvmType,"i1");
    else
        sprintf(llvmType,"i32*");
}

int buildExpression(Expr* expr, int leftExpr, int rightExpr, char* operation)
{
    int returnValue = varNumber++;

    if(expr->expr1->type == INTLIT_T && expr->expr2->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %d, %d\n", returnValue, operation, leftExpr, rightExpr);
    }
    else if(expr->expr1->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %d, %%%d\n", returnValue, operation, leftExpr, rightExpr);
    }
	else if(expr->expr2->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %%%d, %d\n", returnValue, operation, leftExpr, rightExpr);
    }
    else
    {
        printf("\t%%%d = %s i32 %%%d, %%%d\n", returnValue, operation, leftExpr, rightExpr);
    }

    return returnValue;
}




int genExpr(Expr* expr)
{
    int returnValue;

    int leftExprId, rightExprId;

    if(expr->type == BINOP)
    {
        leftExprId = genExpr(expr->expr1);
        rightExprId = genExpr(expr->expr2);
		

        if(expr->op == AND_T){
            leftExprId = genExpr(expr->expr1);
            rightExprId = genExpr(expr->expr2);

            printf("%%%d = icmp ne i32 %%%d, 0\n", varNumber, 69);
            printf("br i1 %%%d, label %%%d, label\n", 69, 69);
        }
        else if(expr->op == OR_T){

        }
		else {
			returnValue = buildExpression(expr, leftExprId, rightExprId, OpTypeLLVM[expr->op]);
		}

        return returnValue;
    }
    else if(expr->type == UNOP)
    {
        int exprId;

        if(expr->op == PLUS) //Not necessary
        {

        }
        else if(expr->op == MINUS)
        {
            exprId = genExpr(expr->expr1);
            returnValue = varNumber++;
            printf("\t%%%d = sub i32 0, %%%d\n", returnValue, 69);
        }
        else if(expr->op == NOT_)
        {

        }
    }
    else if(expr->type == ID_T)
    {
        Type varType = getSymbolFromGlobal(expr->idLit);

		int local = 0;
		if(varType == -1) {
			varType = getSymbolFromLocal(expr->idLit);
			local = 1;
		}
		printf("ID1 VarNumber: %d\n", varNumber);
        if(varType != INTARRAY && varType != BOOLARRAY)
            printf("\t%%%d = load i32* %c%s\n", varNumber, (local == 1) ? '%' : '@' ,expr->idLit);
		printf("ID2 VarNumber: %d\n", varNumber);	
		returnValue = varNumber++;
		printf("ID3 VarNumber: %d\n", varNumber);
    }

    else if(expr->type == NEWINTARR || expr->type == NEWBOOLARR) {
		char exprType[20];
        getExprType(exprType,expr->expr1);
		returnValue = genExpr(expr->expr1);
		
        if(expr->expr1->type==INTLIT_T || expr->expr1->type==BOOLLIT_T){
            printf("\t%%%s = alloca [%d x %s]\n",nome,returnValue,exprType);
			createArray(nome, exprType, returnValue);
        }
		else{
            printf("\t%%%s = alloca [%%%d x %s]\n",nome,returnValue,exprType);
		}
    }
    else if(expr->type == INTLIT_T)
    {
        returnValue = (int) strtol(expr->idLit, NULL, 0);
    }
    else if(expr->type == BOOLLIT_T)
    {
        if(strcmp(expr->idLit, "true") == 0)
            return 1;
        else
            return 0;
    }
    else if(expr->type == CALL)
    {
		ClassTableEntry *metodo = getClassTableEntry(expr->idLit);
		MethodTableEntry *entry = metodo->methodTable->entries;
		
		ArgsList *aux = expr->argsList;
		
		int tempArgsVar[10];
		char argsTempVar[10];
		char *argsType[10];
		int c = -1;
		
		for(; aux != NULL; aux = aux->next){

			tempArgsVar[++c] = genExpr(aux->expr);
			
			//printf("Merda %d\n", aux->expr->type);
			
			if(aux->expr->type == INTLIT_T || aux->expr->type == BOOLLIT_T ){
				argsTempVar[c] = '\0';
				argsType[c] = llvmTypes[(aux->expr->type == INTLIT_T) ? INT_T : BOOL_T];
			}

			else if(aux->expr->type == CALL){
				ClassTableEntry *m = getClassTableEntry(aux->expr->idLit);
				//printf("Merda %d\n", m->type);
				argsType[c] = llvmRetTypes[m->type];
				argsTempVar[c] = '%';
			}
			else{
				if(aux->expr->type == ID_T){
					int t = getSymbolFromLocal(aux->expr->idLit);
					argsType[c] = llvmTypes[t];
					
					if(t == -1){
						argsType[c] = llvmTypes[getSymbolFromGlobal(aux->expr->idLit)];
						argsTempVar[c] = '%';
					}
					else
						argsTempVar[c] = '%';
				}
				else if(aux->expr->type == BINOP){
					Type asd = (aux->expr->expr1->type == INTLIT_T) ? INT_T : BOOL_T;
					argsType[c] = llvmTypes[asd];

					argsTempVar[c] = '%';
				}
				else{
					argsType[c] = llvmTypes[INT_T];
					argsTempVar[c] = '%';
				}
				
			}

		}
		
		printf("\t%%%d = call %s (", varNumber, llvmRetTypes[metodo->type]);
		
		if(entry != NULL){
			MethodTableEntry *aux = entry;
			if(aux != NULL){
				if(aux->isParam){
					printf("%s", llvmTypes[aux->type]);
				}
				aux = aux->next;
			}
			
			for(; aux != NULL; aux = aux->next){
				if(aux->isParam){
					printf(", %s", llvmTypes[aux->type]);
				}
			}
		}
		
		printf(")* @%s(", expr->idLit);
		
		if(c != -1){
			
			printf("%s %c%d", argsType[0], argsTempVar[0], tempArgsVar[0]);
			
		}
		
		int i = 1;
		for(; i <= c; i++)
			printf(", %s %c%d", argsType[i], argsTempVar[i], tempArgsVar[i]);
			
		printf(")\n");

		returnValue = varNumber++;
    }
    else if(expr->type == PARSEINT_T) {
		
		int e = genExpr(expr->expr1);
		
		Type varType = getSymbolFromGlobal(expr->idLit);

		int local = 0;
		if(varType == -1) {
			varType = getSymbolFromLocal(expr->idLit);
			local = 1;
		}
		
		//verificar se é intlit
		if(expr->expr1->type==INTLIT_T || expr->expr1->type==BOOLLIT_T )
            printf("\t%%%d = getelementptr inbounds i8** %c%s, i32 %d\n", varNumber++, (local == 1) ? '%' : '@', expr->idLit, e);
        else
            printf("\t%%%d = getelementptr inbounds i8** %c%s, i32 %%%d\n", varNumber++, (local == 1) ? '%' : '@', expr->idLit, e);
       
		printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber - 1);
		printf("\t%%%d = call i32 @atoi(i8* %%%d) nounwind readonly\n", varNumber++ , varNumber -1);
		returnValue = varNumber -1;
    }
    else if(expr->type == INDEX) {
  
		leftExprId = genExpr(expr->expr1);
		rightExprId = genExpr(expr->expr2);
		int isLocal;
		char varDeclSymbol[5];
		
		Type arrayType = getSymbolFromLocal(expr->expr1->idLit);
        if(arrayType == -1){
            isLocal = 0;
            arrayType = getSymbolFromGlobal(expr->expr1->idLit);
        }
		
		Array *var = getArray(expr->expr1->idLit);

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

		returnValue = varNumber++;
        if(expr->expr2->type == INTLIT_T || expr->expr2->type == BOOLLIT_T ){
			if(var != NULL)
				printf("\t%%%d = getelementptr inbounds [%d x %s]* %s%s, i32 0, i32 %d\n", returnValue, var->size, var->tipo, varDeclSymbol, expr->expr1->idLit, rightExprId);
			else
				printf("\t%%%d = getelementptr inbounds %s %s%s, i32 0, i32 %d\n", returnValue, llvmTypes[arrayType], varDeclSymbol, expr->expr1->idLit, rightExprId);
        
		}
		else{
			if(var != NULL)
				printf("\t%%%d = getelementptr inbounds [%d x %s]* %s%s, i32 0, i32 %%%d\n", returnValue, var->size, var->tipo, varDeclSymbol, expr->expr1->idLit, rightExprId);
			else
				printf("\t%%%d = getelementptr inbounds %s %s%s, i32 %%%d\n", returnValue, llvmTypes[arrayType], varDeclSymbol, expr->expr1->idLit, rightExprId);
		
		}
		
		/*
		if(expr->expr2->type==INTLIT_T || expr->expr2->type==BOOLLIT_T )
			printf("\t%%%d = getelementptr inbounds i32* %%%d, i32 0, i32 %d\n", returnValue, leftExprId, rightExprId);
		else
			printf("\t%%%d = getelementptr inbounds i32* %%%d, i32 0, i32 %%%d\n", returnValue, leftExprId, rightExprId);
			*/
		printf("\t%%%d = load i32* %%%d\n", varNumber, returnValue);
		returnValue = varNumber++;
    }
    
	printf("RET VarNumber: %d\n", varNumber);
	return returnValue;
}

void getTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmTypes[type]);
}

void getRetTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmRetTypes[type]);
}

Array *getArray(char* id)
{
    Array* aux = arraysList;
    for(; aux != NULL; aux = aux->next){
        if(aux->id && (strcmp(id, aux->id) == 0))
            return aux;
    }

    return NULL;
}

void createArray(char *id, char *tipo, int size){
	Array *tail = arraysList;
    for(; (tail != NULL) && (tail->next != NULL); tail = tail->next);
	
	Array *a = (Array*) malloc(sizeof(Array));
	a->id = id;
	strcpy(a->tipo, tipo);
	a->size = size;
	a->next = NULL;
	
	if(arraysList == NULL)
		arraysList = a;
	else
		tail->next = a;
		
	//printf("Arrayteste: %s %d %s\n", arraysList->id, arraysList->size, arraysList->tipo);
}
