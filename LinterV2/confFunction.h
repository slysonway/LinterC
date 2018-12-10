//
// Created by mathi on 2018-11-10.
//

#ifndef LINTERPROJECT_CONFFUNCTION_H
#define LINTERPROJECT_CONFFUNCTION_H

#include "stdio.h"
#include "string.h"
#include "linkedList.h"

typedef struct ConfElement
{
    LinkedList *rules;
    LinkedList *excludedFiles;
} ConfElement;

typedef struct RulesElement
{
    void (*initFunction)(int line, char *data, struct RulesElement *rulesParam, int isEnd, int *error, char *errorMessage);
    void *baseData;
    int temp;
    char rulesName[100];
    int  number;
} RulesElement;

ConfElement *initGetParam(void);

ConfElement *getParam(char *file, ConfElement *newConf);

LinkedList *addRules(LinkedList *rulesList, char *rulesDetail);

#endif //LINTERPROJECT_CONFFUNCTION_H
