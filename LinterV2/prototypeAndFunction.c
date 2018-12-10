//
//  prototypeAndFunction.c
//  LinterV2
//
//  Created by Pierre Kettmus on 07/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#include "prototypeAndFunction.h"

void findPrototype(char *type, RulesElement *rulesParam, char *data, int line) {
    char *tmp = malloc(sizeof(char) * strlen(data));
    
    if (strchr(data, '(') && strchr(data, ')') && strchr(data, ';') && !strchr(data, '=')) {
        strcpy(tmp, data);
        tmp = strtok(tmp, "(");
        if (checkSimilarity(type, tmp)) {
        tmp = tmp + strlen(type);
        addVariable(rulesParam, trim(tmp), line, 0);
        }
    }

    free(tmp);
}

void findFunction(char *type, RulesElement *rulesParam, char *data, int line) {
    char *tmp = malloc(sizeof(char) * strlen(data));
    
    if (strchr(data, '(') && strchr(data, ')') && !strchr(data, ';') && !strchr(data, '=')) {
        strcpy(tmp, data);
        tmp = strtok(tmp, "(");
        if (checkSimilarity(type, tmp)) {
            tmp = tmp + strlen(type);
            if (strchr(tmp, '*')) {
                tmp = strchr(tmp, '*') + 1;
                addVariable(rulesParam, trim(tmp), line, 0);
            } else {
                addVariable(rulesParam, trim(tmp), line, 0);
            }
        }
    }
    free(tmp);
}
