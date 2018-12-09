//
//  prototypeAndFunction.h
//  LinterV2
//
//  Created by Pierre Kettmus on 07/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#ifndef prototypeAndFunction_h
#define prototypeAndFunction_h

#include <stdio.h>
#include "library.h"

void findPrototype(char *type, RulesElement *rulesParam, char *data, int line);
void findFunction(char *type, RulesElement *rulesParam, char *data, int line);

#endif /* prototypeAndFunction_h */
