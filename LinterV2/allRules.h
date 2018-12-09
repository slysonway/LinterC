//
// Created by mathi on 2018-11-10.
//

#ifndef LINTERPROJECT_ALLRULES_H
#define LINTERPROJECT_ALLRULES_H

#include "confFunction.h"
#include "library.h"
#include "variable.h"
#include "prototypeAndFunction.h"

typedef struct Variable {
    char *name;
    int use;
    int global;
    int line;
} Variable;

typedef struct BaseData
{
    char previous[1000];
    int inFunction;
    int inStruct;
    int line;
    char **otherType;
    int otherTypeSize;
    int variableSize;
    Variable* variable;
} BaseData;

void arrayBracketEol(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void operatorsSpacing(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void indent(int line, char *currentLine, RulesElement *rulesParam, int *error, char *errorMessage);

void noMultiDeclaration(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void unusedVariable(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void undeclaredVariable(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void noPrototype(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void unusedFunction(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

void variableAssignmentType(int line, char *data, RulesElement *rulesParam, int *error, char *errorMessage);

#endif //LINTERPROJECT_ALLRULES_H
