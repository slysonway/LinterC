//
//  variable.c
//  LinterV2
//
//  Created by Pierre Kettmus on 07/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#include "variable.h"

//add variable in tab
void addVariable(RulesElement *rulesParam, char *var, int line, int global) {
    if (((BaseData *)rulesParam->baseData)->variableSize == 0) {
        ((BaseData *)rulesParam->baseData)->variable = malloc(sizeof(Variable) * 255);
        
        ((BaseData *)rulesParam->baseData)->variable[0].name = malloc(sizeof(char) * strlen(var));
        ((BaseData *)rulesParam->baseData)->variableSize = 1;
        ((BaseData *)rulesParam->baseData)->variable[0].line = line;
        ((BaseData *)rulesParam->baseData)->variable[0].use = 0;
        ((BaseData *)rulesParam->baseData)->variable[0].global = global;
    } else {
        ((BaseData *)rulesParam->baseData)->variableSize++;
        //printf("%d\n", ((BaseData *)rulesParam->baseData)->variableSize);
        ((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].name = malloc(sizeof(char) * strlen(var));
        ((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].line = line;
        ((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].use = 0;
        ((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].global = global;
    }
    strcpy(((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].name, trim(var));
    //printf("%s\n", ((BaseData *)rulesParam->baseData)->variable[((BaseData *)rulesParam->baseData)->variableSize - 1].name);
}

//check and find variable to add in tab
void findVariable(char *data, RulesElement *rulesParam, int line, int isGlobal) {
    char *tmp;
    int i;
    
    if (data[0] == '(' || data[0] == ')') {
        return;
    }
    
    
    if (strchr(data, '=')) {
        for (i = 0; i < strlen(data); i++) {
            if (data[i] == '=') {
                tmp = calloc(i, sizeof(char));
                strncpy(tmp, data, i);
                if (strchr(tmp, '*')) {
                    tmp = strchr(tmp, '*') + 1;
                }
                addVariable(rulesParam, trim(tmp), line, isGlobal);
                break;
            }
        }
        if (strchr(data, ',')) {
            findVariable(trim(data + myStrlen(data, ',')), rulesParam, line, isGlobal);
        } else {
            return;
        }
    } else {
        if (!strchr(data, '(') && !strchr(data, ')')) {
            if (!strchr(data, ',')) {
                for (i = 0; i < strlen(data); i++) {
                    if (data[i] == ';') {
                        data[i] = '\0';
                        break;
                    }
                }
                addVariable(rulesParam, trim(data), line, isGlobal);
            } else {
                if (strchr(data, ',')) {
                    tmp = calloc(myStrlen(data, ','), sizeof(char));
                    strncpy(tmp, data, myStrlen(data, ',') - 1);
                    addVariable(rulesParam, trim(tmp), line, isGlobal);
                    findVariable(trim(data + myStrlen(data, ',')), rulesParam, line, isGlobal);
                }
            }
        }
    }
}

//erase no global variable from table
void eraseNoGlobalVariable(RulesElement *rulesParam) {
    int i;
    if (((BaseData *)rulesParam->baseData)->variableSize > 0) {
        for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
            if (((BaseData *)rulesParam->baseData)->variable[i].global != 1) {
                eraseInTab(rulesParam, i, ((BaseData *)rulesParam->baseData)->variableSize);
                ((BaseData *)rulesParam->baseData)->variableSize--;
                i--;
            }
        }
    }
}

//check which variable is using table variabl
void searchUseVariable(RulesElement *rulesParam, char *data, int index) {
    Variable var = ((BaseData *)rulesParam->baseData)->variable[index];
    
    if (strstr(data, var.name)) {
        if (checkSimilarity(var.name, data)) {
            ((BaseData *)rulesParam->baseData)->variable[index].use = 1;
        }
    }
}

