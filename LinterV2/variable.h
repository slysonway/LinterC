//
//  variable.h
//  LinterV2
//
//  Created by Pierre Kettmus on 07/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#ifndef variable_h
#define variable_h

#include <stdio.h>
#include "library.h"


void searchUseVariable(RulesElement *rulesParam, char *data, int index);
void eraseNoGlobalVariable(RulesElement *rulesParam);
void addVariable(RulesElement *rulesParam, char *var, int line, int global);
void findVariable(char *data, RulesElement *rulesParam, int line, int global);

#endif /* variable_h */
