#ifndef SHOWS_H
#define SHOWS_H

#include "structures.h"


void printClass(Class* class);
void printDeclList(DeclList* list);
void printMethodDecl(MethodDecl *method);
void printMethodParams(ParamList *params, int level);
void print(char *s, int level, int linebreak);
char* idFormat(char *id);
char* typeToString(Type type);


#endif