//
//  library.h
//  LinterV2
//
//  Created by Pierre Kettmus on 03/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#ifndef library_h
#define library_h

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "allRules.h"
#include "confFunction.h"
#include "variable.h"



void displayTab(char **var, int size);
char *trim (char *str);
void addOtherType(RulesElement *rulesParam, char *var);
void findOtherTypes(char *data, RulesElement *rulesParam);
void functionLevel(RulesElement *rulesParam, char *var);
void functionStart(int typeSize, char **type, RulesElement *rulesParam, char *var);
int myStrlen(char *str, char ch);
void eraseInTab(RulesElement *rulesParam, int pos, int size);
int checkSimilarity(char *compareData, char *data);

#endif /* library_h */
