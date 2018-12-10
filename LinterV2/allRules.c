//
// Created by mathi on 2018-11-10.
//

#include "allRules.h"

void arrayBracketEol(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage)
{
    //puts("EOL");
}

void operatorsSpacing(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage)
{
    size_t length = strlen(data);
    char allOperator[] = {'=', '+', '-', '/', '*', '&', '|'};
    int errorFind = 0;

    for(int i = 0; i < length; i++)
    {
        if(i > 0 && i < length - 1)
        {
            for (int j = 0; j < 7; ++j)
            {
                if(data[i] == allOperator[j] && i > 0 && (data[i - 1] != ' ' || data[i + 1] != ' '))
                {
                    strcat(errorMessage, "        The rule operator spacing is not respected \n");

                    errorFind = 1;
                    *error = 1;
                    break;
                }
            }
        }

        if(errorFind == 1)
            break;
    }
}

void indent(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage){

    //printf("%d \n", rulesParam->number);
    
}



void noMultiDeclaration(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage) {
    //printf("%s", data);
    char *type[] = {"char", "short int", "short", "int", "long int", "long", "long long int", "long long", "float", "double", "long double", "struct"};
    int i;
    for (i = 0; i < 12; i++) {
        if (strstr(data, type[i])) {
            if (strstr(data, ",") && strstr(data, "(") == NULL) {
                strcat(errorMessage, "\t\tThe rule no multi declaration is not respected\n");
                *error = 1;
                break;
            }
        }
    }
}

//TODO Check global variable
//check unusedVariable in c file
void unusedVariable(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage) {
    char *type[] = {"char", "short int", "short", "int", "long int", "long", "long long int", "long long", "float", "double", "long double", "void"};
    int i;
    int j;
    int typeSize = 12;
    char *tmp;
    char errorBuffer[255];
    
    //init needed variable
    if (rulesParam->baseData == NULL || line == 0) {
        if(rulesParam->baseData != NULL) {
            free(rulesParam->baseData);
        }
        rulesParam->baseData = malloc(sizeof(BaseData));
        ((BaseData *)rulesParam->baseData)->inFunction = 0;
        ((BaseData *)rulesParam->baseData)->inStruct = 0;
        ((BaseData *)rulesParam->baseData)->otherTypeSize = 0;
        ((BaseData *)rulesParam->baseData)->variableSize = 0;
    }
    
    //start procress in new struct
    if (rulesParam->baseData != NULL) {
        findOtherTypes(data, rulesParam);
        
        if (line > 51) {
            printf("");
        }
        
        //check in which level we are
        functionLevel(rulesParam, data);
        //check if we are in function with normal type
        if (((BaseData *)rulesParam->baseData)->inFunction == 0) {
            functionStart(typeSize, type, rulesParam, data);
        //now with new type from struct
            functionStart(((BaseData *)rulesParam->baseData)->otherTypeSize, ((BaseData *)rulesParam->baseData)->otherType, rulesParam, data);
        }
        
        //for DEBUG display level of function
        //printf("line : %d, value: %d\n", line, ((BaseData *)rulesParam->baseData)->inFunction);
        
        
        //process find variable
        if (((BaseData *)rulesParam->baseData)->inFunction > 0) {
            //for normal types
            for (i = 0; i  < typeSize; i++) {
                if (strstr(data, type[i]) && ((BaseData *)rulesParam->baseData)->inStruct == 0) {
                    if (checkSimilarity(type[i], data)) {
                        tmp = strstr(data, type[i]) + strlen(type[i]);
                        findVariable(trim(tmp), rulesParam, line, 0);
                    }
                    //printf("%d", ((BaseData *)rulesParam->baseData)->variableSize);
                }
            }
            //for other type like struct
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
                if (strstr(data, ((BaseData *)rulesParam->baseData)->otherType[i]) && ((BaseData *)rulesParam->baseData)->inStruct == 0) {
                    if (checkSimilarity(((BaseData *)rulesParam->baseData)->otherType[i], data)) {
                        tmp = strstr(data,  ((BaseData *)rulesParam->baseData)->otherType[i]) + strlen( ((BaseData *)rulesParam->baseData)->otherType[i]);
                        findVariable(trim(tmp), rulesParam, line, 0);
                    }
                    //printf("%d", ((BaseData *)rulesParam->baseData)->variableSize);
                }
            }
            
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
                searchUseVariable(rulesParam, data, i);
            }
            
        //find global variable
        } else {
            //handle error if variable not global and not use if variableSize > 0
            if (((BaseData *)rulesParam->baseData)->variableSize > 0) {
                for (j = 0; j < ((BaseData *)rulesParam->baseData)->variableSize; j++) {
                    if (((BaseData *)rulesParam->baseData)->variable[j].global != 1 && ((BaseData *)rulesParam->baseData)->variable[j].use != 1) {
                        sprintf(errorBuffer, "\t\tThe rule unused variable is not respected at line: %d\n", ((BaseData *)rulesParam->baseData)->variable[j].line);
                        strcat(errorMessage, errorBuffer);
                        *error = 1;
                    }
                }
            }
            //erase no global variable because we are out of function
            eraseNoGlobalVariable(rulesParam);
            for (i = 0; i < typeSize; i++) {
                if (strstr(data, type[i]) && ((BaseData *)rulesParam->baseData)->inStruct == 0) {
                    if (checkSimilarity(type[i], data)) {
                        tmp = strstr(data, type[i]) + strlen(type[i]);
                        findVariable(trim(tmp), rulesParam, line, 1);
                    }
                    //printf("%d", ((BaseData *)rulesParam->baseData)->variableSize);
                }
            }
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
                if (strstr(data, ((BaseData *)rulesParam->baseData)->otherType[i]) && ((BaseData *)rulesParam->baseData)->inStruct == 0) {
                    if (checkSimilarity(((BaseData *)rulesParam->baseData)->otherType[i], data)) {
                        tmp = strstr(data,  ((BaseData *)rulesParam->baseData)->otherType[i]) + strlen( ((BaseData *)rulesParam->baseData)->otherType[i]);
                        findVariable(trim(tmp), rulesParam, line, 1);
                    }
                }
            }
        }
        
        if (isEnd) {
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
                if (((BaseData *)rulesParam->baseData)->variable[i].use < 1) {
                    sprintf(errorBuffer, "\t\tThe rule unused variable is not respected at line: %d\n", ((BaseData *)rulesParam->baseData)->variable[i].line);
                    strcat(errorMessage, errorBuffer);
                    *error = 1;
                }
                free(((BaseData *)rulesParam->baseData)->variable[i].name);
            }
            free(((BaseData *)rulesParam->baseData)->variable);
            
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
                free(((BaseData *)rulesParam->baseData)->otherType[i]);
            }
            free(((BaseData *)rulesParam->baseData)->otherType);
            
            free((BaseData *)rulesParam->baseData);
        }
    }
}


void undeclaredVariable(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage) {
    
}

void noPrototype(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage) {
    char *type[] = {"char", "short int", "short", "int", "long int", "long", "long long int", "long long", "float", "double", "long double", "void"};
    int i;
    int j;
    int typeSize = 12;
    char *tmp = malloc(sizeof(char) * strlen(data));
    int isUse = 0;

    
    //init needed variable
    if (rulesParam->baseData == NULL || line == 0) {
        if(rulesParam->baseData != NULL) {
            free(rulesParam->baseData);
        }
        rulesParam->baseData = malloc(sizeof(BaseData));
        ((BaseData *)rulesParam->baseData)->inFunction = 0;
        ((BaseData *)rulesParam->baseData)->inStruct = 0;
        ((BaseData *)rulesParam->baseData)->otherTypeSize = 0;
        ((BaseData *)rulesParam->baseData)->variableSize = 0;
    }
    
    //start process
    if (rulesParam->baseData != NULL) {
        findOtherTypes(data, rulesParam);
        
        //search prototype with normal type
        for (i = 0; i < typeSize; i++) {
            if (strstr(data, type[i])) {
                findPrototype(type[i], rulesParam, data, line);
            }
        }
        //search prototype with other type
        for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
            if (strstr(data, ((BaseData *)rulesParam->baseData)->otherType[i])) {
                findPrototype(((BaseData *)rulesParam->baseData)->otherType[i], rulesParam, data, line);
            }
        }
        
        //search function with normal type and said if it has a prototype
        for (i = 0; i < typeSize; i++) {
            if (strstr(data, type[i])) {
                if (strchr(data, '(') && strchr(data, ')') && !strchr(data, '=') && !strchr(data, ';')) {
                    //tmp = data + strlen(type[i]);
                    strcpy(tmp, data);
                    tmp = strtok(tmp, "(");
                    if (checkSimilarity(type[i], tmp)) {
                        tmp = tmp + strlen(type[i]);
                        if (((BaseData *)rulesParam->baseData)->variableSize > 0) {
                            for (j = 0; j < ((BaseData *)rulesParam->baseData)->variableSize; j++) {
                                if (strstr(tmp, ((BaseData *)rulesParam->baseData)->variable[j].name)) {
                                    isUse = 1;
                                }
                            }
                            if (isUse != 1) {
                                isUse = 0;
                                strcat(errorMessage, "\t\tThe rule no prototype is not respected\n");
                                *error = 1;
                            }
                        }else {
                            strcat(errorMessage, "\t\tThe rule no prototype is not respected\n");
                            *error = 1;
                        }
                    }
                }
            }
        }
        
        //search function with other type and said if it has a prototype
        for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
            if (strstr(data, ((BaseData *)rulesParam->baseData)->otherType[i])) {
                if (strchr(data, '(') && strchr(data, ')') && !strchr(data, '=') && !strchr(data, ';')) {
                    //tmp = data + strlen(((BaseData *)rulesParam->baseData)->otherType[i]);
                    strcpy(tmp, data);
                    tmp = strtok(tmp, "(");
                    if (checkSimilarity(((BaseData *)rulesParam->baseData)->otherType[i], tmp)) {
                        tmp = tmp + strlen(((BaseData *)rulesParam->baseData)->otherType[i]);
                    
                        if (((BaseData *)rulesParam->baseData)->variableSize > 0) {
                            for (j = 0; j < ((BaseData *)rulesParam->baseData)->variableSize; j++) {
                                if (strstr(tmp, ((BaseData *)rulesParam->baseData)->variable[j].name)) {
                                    isUse = 1;
                                }
                            }
                            if (isUse != 1) {
                                isUse = 0;
                                strcat(errorMessage, "\t\tThe rule no prototype is not respected\n");
                                *error = 1;
                            }
                        }else {
                            strcat(errorMessage, "\t\tThe rule no prototype is not respected\n");
                            *error = 1;
                        }
                    }
                }
            }
        }
        
        if (isEnd) {
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
                free(((BaseData *)rulesParam->baseData)->variable[i].name);
            }
            free(((BaseData *)rulesParam->baseData)->variable);
            
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
                free(((BaseData *)rulesParam->baseData)->otherType[i]);
            }
            free(((BaseData *)rulesParam->baseData)->otherType);
            
            free((BaseData *)rulesParam->baseData);
            free(tmp);
        }
        
    }
}

//TODO check not use at end of file
void unusedFunction(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage) {
    char *type[] = {"char", "short int", "short", "int", "long int", "long", "long long int", "long long", "float", "double", "long double", "void"};
    int i;
    int typeSize = 12;
    char errorBuffer[255];
    
    
    //init needed variable
    if (rulesParam->baseData == NULL || line == 0) {
        if(rulesParam->baseData != NULL) {
            free(rulesParam->baseData);
        }
        rulesParam->baseData = malloc(sizeof(BaseData));
        ((BaseData *)rulesParam->baseData)->inFunction = 0;
        ((BaseData *)rulesParam->baseData)->inStruct = 0;
        ((BaseData *)rulesParam->baseData)->otherTypeSize = 0;
        ((BaseData *)rulesParam->baseData)->variableSize = 0;
    }
    
    
    if (rulesParam->baseData != NULL) {
        findOtherTypes(data, rulesParam);
        
        //find declared function with normal type
        for (i = 0; i < typeSize; i++) {
            if (strstr(data, type[i])) {
                findFunction(type[i], rulesParam, data, line);
            }
        }
        //find declared function with other type
        for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
            if (strstr(data, ((BaseData *)rulesParam->baseData)->otherType[i])) {
                findFunction(((BaseData *)rulesParam->baseData)->otherType[i], rulesParam, data, line);
            }
        }
        
        //find wich function is used
        for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
            if (strstr(data, ((BaseData *)rulesParam->baseData)->variable[i].name) && ((BaseData *)rulesParam->baseData)->variable[i].line != line && strchr(data, '(')) {
                ((BaseData *)rulesParam->baseData)->variable[i].use = 1;
            }
        }
        
        if (isEnd) {
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->variableSize; i++) {
                if (((BaseData *)rulesParam->baseData)->variable[i].use < 1) {
                    sprintf(errorBuffer, "\t\tThe rule unused function is not respected at line: %d\n", ((BaseData *)rulesParam->baseData)->variable[i].line);
                    strcat(errorMessage, errorBuffer);
                    *error = 1;
                }
                free(((BaseData *)rulesParam->baseData)->variable[i].name);
            }
            free(((BaseData *)rulesParam->baseData)->variable);
            
            for (i = 0; i < ((BaseData *)rulesParam->baseData)->otherTypeSize; i++) {
                free(((BaseData *)rulesParam->baseData)->otherType[i]);
            }
            free(((BaseData *)rulesParam->baseData)->otherType);
            
            free((BaseData *)rulesParam->baseData);
        }
    }
}

void variableAssignmentType(int line, char *data, RulesElement *rulesParam, int isEnd, int *error, char *errorMessage)
{
    char *integerType[]     =   {"char", "short int", "short", "int", "long int", "long", "long long int", "long long"};
    char *intergerOption[]  =   {"signed", "unsigned", ""};
    char *realType[]        =   {"float", "double", "long double"};
    char *otherType[]       =   {"void"};
    char *pointeur[]        =   {"*", ""};
    int inFunction = 0;
    BaseData *baseData = NULL;

    if(rulesParam->baseData == NULL || line == 0)
    {
        if(rulesParam->baseData != NULL)

            free(rulesParam->baseData);

        rulesParam->baseData = malloc(1 * sizeof(BaseData));

        ((BaseData *)rulesParam->baseData)->inFunction = 0;
    }

    baseData = rulesParam->baseData;

    if(baseData->inFunction == 0)
    {
        // Check if it's a start function
        //puts(strchr(data, ' '));

    }

    //puts(data);
}
