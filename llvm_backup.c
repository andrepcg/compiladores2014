#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm.h"
#include "symbol_table.h"

#define MAX_LLVM_TYPE_SIZE 15

MethodTable* currentLocalTable;

Type curFunctionType;
int curFunctionisMain;
int varNumber, ifNumber, whileNumber, indexNumber, andNumber, orNumber;


char* llvmTypes[7] = {"void", "i32", "i1", "i32*", "i1*", "i8**", "i8***"};
char* OpTypeLLVM[11] = {"add", "sub", "mul", "sdiv", "srem", "icmp slt", "icmp sgt", "icmp sle", "icmp sge", "icmp ne", "icmp eq"};
char* llvmRetTypes[6] = {"void", "i32", "i1", "{i32, i32*}", "{i32, i1*}", "i8**"};

char *nome;
int returned = 0;
Array *arraysList;
TempArg *argumentosTemporarios;

void generateLLVM(Class* programa)
{
	printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*) nounwind readonly\n\n");
    printf("@.str0 = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
	printf("@str.false = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n");
	printf("@str.true = private unnamed_addr constant [7 x i8] c\"true\\0A\\00\\00\"\n\n");
	printf("@str.bool = global [2 x i8*] [i8* getelementptr inbounds ([7 x i8]* @str.false, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8]* @str.true, i32 0, i32 0)]\n");

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
	
	curFunctionisMain = (strcmp(methodDecl->id, "main") == 0) ? 1 : 0;

    varNumber = ifNumber = whileNumber = indexNumber = andNumber = orNumber = 1;
    currentLocalTable = getLocalTable(methodDecl->id);


    printf("define %s @%s(", llvmRetTypes[methodDecl->tipo], methodDecl->id);

    ParamList* aux = methodDecl->parametros;
    if(aux != NULL)
    {
        if(aux->tipo == INTARRAY || aux->tipo == BOOLARRAY || aux->tipo == STRINGARRAY)
            printf("i32 %%.%s, ", aux->id);
        printf("%s %%%s", llvmTypes[aux->tipo], aux->id);
        aux = aux->next;
    }
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->tipo == INTARRAY || aux->tipo == BOOLARRAY || aux->tipo == STRINGARRAY)
            printf(", i32 %%.%s", aux->id);
        printf(", %s %%%s", llvmTypes[aux->tipo], aux->id);
    }

    printf(") ");
    
    if(curFunctionisMain)
        printf("nounwind uwtable");
    
    printf(" {\n");
	
	//if(!curFunctionisMain){
		aux = methodDecl->parametros;
		for(; aux != NULL; aux = aux->next){
			createTempArg(aux->id, aux->tipo, varNumber++);

			printf("\t%%%d = alloca %s\n", varNumber - 1, llvmTypes[aux->tipo]);
			printf("\tstore %s %%%s, %s %%%d\n", llvmTypes[aux->tipo], aux->id, llvmTypes[(aux->tipo == STRINGARRAY) ? aux->tipo + 1 : aux->tipo + 2], varNumber - 1);

		}
	//}
	
	//printf("Arg %s %d\n", argumentosTemporarios->id, argumentosTemporarios->tempVar);

    VarDeclList* aux2 = methodDecl->declaracoes;
    for(; aux2 != NULL; aux2 = aux2->next)
        genLocalVar(aux2->declaracao);

    genStmtList(methodDecl->stmts);
	
	if(!returned)
		//if(curFunctionisMain)
			//printf("\tret i32 0\n");
		//else
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
        if(stmt->expr1->type==ID_T){
            printf("\tbr i1 %%%d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber-1, ifNumber, ifNumber);
        }
        else if(stmt->expr1->type!=BOOLLIT_T && stmt->expr1->type!=INTLIT_T )
            printf("\tbr i1 %%%d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber, ifNumber, ifNumber); //mudei o compVarNumber
        
        else
             printf("\tbr i1 %d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber, ifNumber, ifNumber);
 
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
			printf("\tret void\n");
		
			
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
		if(stmt->expr1->type == ID_T)
			t = getSymbolFromLocalOrGlobal(stmt->expr1->idLit);

		

		if(t == BOOL_T || (stmt->expr1->expr1 != NULL && (strcmp(stmt->expr1->expr1->idLit, "true") == 0 || strcmp(stmt->expr1->expr1->idLit, "false") == 0))){
			printf("\t%%%d = zext i1 %%%d to i32\n", varNumber++, ret - 1);
			printf("\t%%%d = getelementptr inbounds [2 x i8*]* @str.bool, i32 0, i32 %%%d\n", varNumber++, varNumber-1);
			printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber-1);
			printf("\t%%%d = call i32 (i8*, ...)* @printf(i8* %%%d)\n", varNumber, varNumber-1);
			
		}
		else if(t == INT_T)
			// alterei de (ret - 1) para apenas (ret)
			printf("\t%%%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str0, i32 0, i32 0), i32 %%%d)\n", varNumber, ret);

		varNumber++;
	}
    else if(stmt->tipo == STORE)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char exprType[20];

        if(stmt->expr1->type==NEWINTARR || stmt->expr1->type==NEWBOOLARR)
            nome=stmt->id;

        int exprVarNumber = genExpr(stmt->expr1);

        Type varType = getSymbolFromLocal(stmt->id);
      
        if(varType == -1){
            isLocal = 0;
            varType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        getExprType(exprType,stmt->expr1);
        if(stmt->expr1->type!=NEWINTARR && stmt->expr1->type!=NEWBOOLARR){
			//printf("Teste: %d %d\n", varType, stmt->expr1->type);
            if(stmt->expr1->type==INTLIT_T ||stmt->expr1->type==BOOLLIT_T )
                printf("\tstore %s %d, %s* %s%s\n", exprType, exprVarNumber, llvmTypes[varType], varDeclSymbol, stmt->id);
            else{

				printf("\tstore %s %%%d, %s* %s%s\n", exprType, exprVarNumber - 1, llvmTypes[varType], varDeclSymbol, stmt->id);
			}
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
				printf("\t%%ind%d = getelementptr inbounds [%d x %s]* %s%s, i32 0, i32 %d\n", indexNumber, var->size, var->tipo, varDeclSymbol, stmt->id, indexVarNumber);
			else
				printf("\t%%ind%d = getelementptr inbounds %s %s%s, i32 %d\n", indexNumber, llvmTypes[arrayType], varDeclSymbol, stmt->id, indexVarNumber);
        
		}
		else
            printf("\t%%ind%d = getelementptr inbounds %s %s%s, i32 %%%d\n", indexNumber, llvmTypes[arrayType], varDeclSymbol, stmt->id, indexVarNumber);
       
		if(stmt->expr2->type==INTLIT_T ||stmt->expr2->type==BOOLLIT_T )
			printf("\tstore %s %d, %s %%ind%d\n", llvmTypes[arrayType - 2], exprVarNumber, llvmTypes[arrayType], indexNumber);
        else
			printf("\tstore %s %%%d, %s %%ind%d\n", llvmTypes[arrayType - 2], exprVarNumber, llvmTypes[arrayType], indexNumber);
		indexNumber++;
    }
	
}

void getExprType(char* llvmType, Expr* expr){
    if(expr->type == INTLIT_T || expr->type == MINUS || expr->type == ID_T)
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
            int tempId,tempTrue,tempTrap;
 
            leftExprId = genExpr(expr->expr1);
 
            tempId = varNumber++;
            if(expr->expr1->type==BOOLLIT_T )
                printf("\t%%%d = icmp ne i1 %d, 0\n", tempId, leftExprId);
            else
                printf("\t%%%d = icmp ne i1 %%%d, 0\n", tempId, leftExprId);
            printf("\tbr i1 %%%d, label %%andTrue%d, label %%andTrap%d\n\n", tempId, andNumber, andNumber);
 
            printf("andTrue%d:\n", andNumber);
            rightExprId = genExpr(expr->expr2);
            if(expr->expr2->type==BOOLLIT_T )
                printf("%%%d = icmp ne i1 %d, 0\n", varNumber, rightExprId);
            else
                printf("%%%d = icmp ne i1 %%%d, 0\n", varNumber, rightExprId);
            tempTrue=varNumber;
            returnValue=varNumber;
            tempId = varNumber++;
            printf("\tbr label %%andFalse%d\n\n", andNumber);
            printf("andTrap%d:\n",andNumber);
            printf("\t%%%d = icmp ne i1 0, 0\n",varNumber);
            tempTrap=varNumber;
            varNumber++;
            printf("\tbr label %%andFalse%d\n\n", andNumber);
            printf("andFalse%d:\n", andNumber);
            printf("\t%%%d = phi i1 [ %%%d, %%andTrue%d ], [ %%%d, %%andTrap%d ]\n",varNumber,tempTrue,orNumber,tempTrap,orNumber);
 
            andNumber++;
            returnValue=varNumber++;
        }
        else if(expr->op == OR_T){
            int tempId,tempTrap,tempTrue;
 
            leftExprId = genExpr(expr->expr1);
 
            tempId = varNumber++;
            if(expr->expr1->type==BOOLLIT_T )
                printf("\t%%%d = icmp ne i1 %d, 0\n", tempId, leftExprId);
            else
                printf("\t%%%d = icmp ne i1 %%%d, 0\n", tempId, leftExprId);
            printf("\tbr i1 %%%d, label %%orTrue%d, label %%orTrap%d\n\n", tempId, orNumber, orNumber);

            printf("orTrap%d:\n",orNumber);
            rightExprId = genExpr(expr->expr2);
            if(expr->expr2->type==BOOLLIT_T )
                printf("\t%%%d = icmp ne i1 %d, 0\n", varNumber, rightExprId);
            else
                printf("\t%%%d = icmp ne i1 %%%d, 0\n", varNumber, rightExprId);
            tempTrap=varNumber;
            returnValue=varNumber;
            tempId = varNumber++;
           
            printf("\tbr label %%orFalse%d\n\n", orNumber);

            printf("orTrue%d:\n", orNumber);
            printf("\t%%%d = icmp ne i1 1, 0\n",varNumber);
            printf("\tbr label %%orFalse%d\n\n", orNumber);
            tempTrue=varNumber;
            varNumber++;
 
            printf("orFalse%d:\n", orNumber);
            printf("\t%%%d = phi i1 [ %%%d, %%orTrue%d ], [ %%%d, %%orTrap%d ]\n",varNumber,tempTrue,orNumber,tempTrap,orNumber);
            returnValue=varNumber++;
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
            if(expr->expr1->type==INTLIT_T)
                printf("\t%%%d = sub i32 0, %d\n", returnValue, exprId);
            else
                printf("\t%%%d = sub i32 0, %%%d\n", returnValue, exprId);
        }
        else if(expr->op == NOT_)//VER ESTA MERDA DO NOT CARALHOOO
        {
            int isLocal=1;
            exprId = genExpr(expr->expr1);
            returnValue = varNumber++;
            Type varType = getSymbolFromLocal(expr->expr1->idLit);
            //printf("varType %s",varType);
            /*if(expr->expr1->type==INTLIT_T) 
                printf("%%%d = icmp sle i1 %d, 0\n",returnValue,exprId);*/
            if(expr->expr1->type==BOOLLIT_T)
                printf("%%%d = icmp ne i1 %d, 1\n",returnValue,exprId);
            else if(expr->expr1->type==ID_T){
                if(varType==BOOL_T){
                    printf("%%%d = icmp ne i1 %%%d, 1\n",returnValue,exprId);
                }
                else
                    printf("%%%d = icmp sle i1 %%%d, 0\n",returnValue,exprId);//Isto provavelmente é para tirar, pq nao ha NOT sobre inteiros
            }
            else
                printf("%%%d = icmp ne i1 %%%d, 1\n",returnValue,exprId);//Aqui ta problema, quando é NOT de expressao
        }
    }
    else if(expr->type == ID_T)
    {
		TempArg *arg = getTempArg(expr->idLit);

		if(arg == NULL){
			Type varType = getSymbolFromGlobal(expr->idLit);
			int local = 0;
			if(varType == -1) {
				varType = getSymbolFromLocal(expr->idLit);
				local = 1;
			}

			if(varType != INTARRAY && varType != BOOLARRAY){
				printf("\t%%%d = load %s %c%s\n", varNumber++, llvmTypes[varType + 2], (local == 1) ? '%' : '@' ,expr->idLit);
				
			}
		}
		else{
			printf("\t%%%d = load %s %%%d\n", varNumber++, llvmTypes[arg->tipo + 2], arg->tempVar);
		}
		
		returnValue = varNumber;
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
		int isLocal = 1;
		int e = genExpr(expr->expr1);
		
		Type varType = getSymbolFromLocal(expr->idLit);
        if(varType == -1){
            isLocal = 0;
            varType = getSymbolFromGlobal(expr->idLit);
        }
		
		char t[15];
		TempArg *arg = getTempArg(expr->idLit);
		if(arg != NULL){
			printf("\t%%%d = load %s %%%d\n", varNumber++, llvmTypes[(arg->tipo == STRINGARRAY) ? arg->tipo + 1 : arg->tipo + 2], arg->tempVar);
			sprintf(t, "%%%d", varNumber - 1);
		}
		else
			sprintf(t, "%c%s", (isLocal == 1) ? '%' : '@', expr->idLit);
			
		e = (strcmp("args", expr->idLit) == 0) ? e + 1 : e;
		

		if(expr->expr1->type==INTLIT_T || expr->expr1->type==BOOLLIT_T )
            printf("\t%%%d = getelementptr inbounds i8** %s, i32 %d\n", varNumber++, t, e);
        else
            printf("\t%%%d = getelementptr inbounds i8** %s, i32 %%%d\n", varNumber++, t, e);
       
		printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber - 1);
		printf("\t%%%d = call i32 @atoi(i8* %%%d) nounwind readonly\n", varNumber++ , varNumber -1);
		returnValue = varNumber -1;
    }
    else if(expr->type == INDEX) {
  
		leftExprId = genExpr(expr->expr1);
		rightExprId = genExpr(expr->expr2);
		int isLocal = 1;
		char varDeclSymbol[5];
		char t[15];
		
		Type arrayType = getSymbolFromLocal(expr->expr1->idLit);
        if(arrayType == -1){
            isLocal = 0;
            arrayType = getSymbolFromGlobal(expr->expr1->idLit);
        }
		
		Array *var = getArray(expr->expr1->idLit);

		TempArg *arg = getTempArg(expr->expr1->idLit);
		if(arg != NULL)
			sprintf(t, "%%%d", arg->tempVar);
		else
			sprintf(t, "%c%s", (isLocal == 1) ? '%' : '@', expr->expr1->idLit);


		returnValue = varNumber++;
        if(expr->expr2->type == INTLIT_T || expr->expr2->type == BOOLLIT_T ){
			
			if(var != NULL)
				printf("\t%%%d = getelementptr inbounds [%d x %s]* %s, i32 0, i32 %d\n", returnValue, var->size, var->tipo, t, rightExprId);
			else
				printf("\t%%%d = getelementptr inbounds %s %s, i32 0, i32 %d\n", returnValue, llvmTypes[arrayType], t, rightExprId);
        
		}
		else{
			if(var != NULL)
				printf("\t%%%d = getelementptr inbounds [%d x %s]* %s, i32 0, i32 %%%d\n", returnValue, var->size, var->tipo, t, rightExprId - 1);
			else
				printf("\t%%%d = getelementptr inbounds %s %s, i32 %%%d\n", returnValue, llvmTypes[arrayType], t, rightExprId -1);
		
		}
		

		printf("\t%%%d = load i32* %%%d\n", varNumber, returnValue);
		returnValue = varNumber++;
    }

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

TempArg *getTempArg(char* id)
{
    TempArg* aux = argumentosTemporarios;
    for(; aux != NULL; aux = aux->next){
        if(strcmp(id, aux->id) == 0){
            return aux;
		}
    }

    return NULL;
}

void createTempArg(char *id, Type tipo, int tempVar){
	TempArg *tail = argumentosTemporarios;
    for(; (tail != NULL) && (tail->next != NULL); tail = tail->next);
	
	TempArg *a = (TempArg*) malloc(sizeof(TempArg));
	a->id = id;
	a->tipo = tipo;
	a->tempVar = tempVar;
	a->next = NULL;
	
	if(argumentosTemporarios == NULL)
		argumentosTemporarios = a;
	else
		tail->next = a;
}
