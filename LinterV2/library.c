//
//  library.c
//  LinterV2
//
//  Created by Pierre Kettmus on 03/12/2018.
//  Copyright © 2018 Pierre Kettmus. All rights reserved.
//

#include "library.h"


//display string table for debug
void displayTab(char **var, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", var[i]);
    }

}

//remove all space
char *trim (char *str)
{
    char *ibuf, *obuf;
    
    if (str)
    {
        for (ibuf = obuf = str; *ibuf; )
        {
            while (*ibuf && (isspace(*ibuf)))
                ibuf++;
            if (*ibuf && (obuf != str))
                *(obuf++) = ' ';
            while (*ibuf && (!isspace(*ibuf)))
                *(obuf++) = *(ibuf++);
        }
        *obuf = '\0';
    }
    return (str);
}


//find new type generate by type def struct
void findOtherTypes(char *data, RulesElement *rulesParam) {
    char *tmp;
    int i;
    
    //find struct to add in variable Type
    if (strstr(data, "typedef struct") || ((BaseData *)rulesParam->baseData)->inStruct == 1) {
        ((BaseData *)rulesParam->baseData)->inStruct = 1;
        //125 = }
        if ((tmp = strchr(data, 125))) {
            tmp = strchr(data, 125) + 1;
            
            for ( i = 0; i < strlen(tmp); i++) {
                if (tmp[i] == ';') {
                    tmp[i] = '\0';
                    break;
                }
            }
            //add other type dynamicly to char**
            addOtherType(rulesParam, tmp);
            ((BaseData *)rulesParam->baseData)->inStruct = 0;
            return;
        }
    }
}


//add other type in tab
void addOtherType(RulesElement *rulesParam, char *var) {
    if (((BaseData *)rulesParam->baseData)->otherTypeSize == 0) {
        ((BaseData *)rulesParam->baseData)->otherType = (char **)malloc(sizeof(char *));
        ((BaseData *)rulesParam->baseData)->otherTypeSize = 1;
        ((BaseData *)rulesParam->baseData)->otherType[0] = malloc(sizeof(char) * strlen(var));
        
    } else { //realloc ptr for adding type
        ((BaseData *)rulesParam->baseData)->otherTypeSize++;
        if (realloc(((BaseData *)rulesParam->baseData)->otherType, sizeof(char *) * ((BaseData *)rulesParam->baseData)->otherTypeSize) != NULL) {
            ((BaseData *)rulesParam->baseData)->otherType[((BaseData *)rulesParam->baseData)->otherTypeSize - 1] = malloc(sizeof(char) * strlen(var));
        }
    }
    //cpy new type in last case of pointer
    strcpy(((BaseData *)rulesParam->baseData)->otherType[((BaseData *)rulesParam->baseData)->otherTypeSize - 1], trim(var));
    //printf("%s\n", ((BaseData *)rulesParam->baseData)->otherType[((BaseData *)rulesParam->baseData)->otherTypeSize - 1]);
}

//check level of funtion
/**
 0 = no
 1 or more = yes
 -1 = maybe;
 **/
void functionLevel(RulesElement *rulesParam, char *var) {
    // 123 = {
    if (strchr(var, 123) && ((BaseData *)rulesParam->baseData)->inFunction == -1) {
        ((BaseData *)rulesParam->baseData)->inFunction = 1;
        return;
    }
    
    //123 = {
    if (strchr(var, 123) && ((BaseData *)rulesParam->baseData)->inFunction > 0) {
        ((BaseData *)rulesParam->baseData)->inFunction++;
    }
    //125 = }
    if (strchr(var, 125)) {
        if (((BaseData *)rulesParam->baseData)->inFunction > 1) {
            ((BaseData *)rulesParam->baseData)->inFunction--;
        } else {
            ((BaseData *)rulesParam->baseData)->inFunction = 0;
        }
    }
    
    
}


//find if we are in function or maybe or no
/**
 0 = no
 1 or more = yes
 -1 = maybe;
 **/
void functionStart(int typeSize, char **type, RulesElement *rulesParam, char *var) {
    int i;
    for (i = 0; i < typeSize; i++) {
        if (strstr(var, type[i]) && strchr(var, '(') && strchr(var, ')') && !strchr(var, '=')) {
            //123 = {
            if (strchr(var, 123)) {
                ((BaseData *)rulesParam->baseData)->inFunction = 1;
                return;
            }else if (strchr(var, ';')) {
                ((BaseData *)rulesParam->baseData)->inFunction = 0;
            }else {
                ((BaseData *)rulesParam->baseData)->inFunction = -1;
            }
        }
    }
}

/*size of
 @param str
until find
 @param ch*/
int myStrlen(char *str, char ch) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == ch) {
            return  i + 1;
        }
    }
    return 0;
}



//erase case in table at pos
void eraseInTab(RulesElement *rulesParam, int pos, int size) {
    int i;
    for (i = 0; i < (size - 1); i++) {
        if ( i >= pos) {
            ((BaseData *)rulesParam->baseData)->variable[i] = ((BaseData *)rulesParam->baseData)->variable[i + 1];
        }
    }
}


//check if data really contain exacly compareData with no difference before and after
//return 0 if compareData and data not equal
//return 1 if compareData and data are equal
int checkSimilarity(char *compareData, char *data) {
    char *tmp;
    //data without compareData
    if (!strstr(data, compareData)) {
        return 0;
    }
    tmp = strstr(data, compareData) + strlen(compareData);
    
    //size of dara with out tmp
    int size = (int)strlen(data) - (int)strlen(tmp);
    
    //if data have char befor compareData
    if (size > strlen(compareData)) {
        //if char before compareData is not a lettre or number or ' or " or _
        if (!(data[size - strlen(compareData) - 1] > 47 && data[size - strlen(compareData) - 1] < 58) && !(data[size - strlen(compareData) - 1] > 64 && data[size - strlen(compareData) - 1] < 91) && !(data[size - strlen(compareData) - 1] > 96 && data[size - strlen(compareData) - 1] < 123) && data[size - strlen(compareData) - 1] != '_' && data[size - strlen(compareData) - 1] != 39 && data[size - strlen(compareData) - 1] != 34) {
            //if data have cher after compareData
            if (strlen(tmp) > 0) {
                //if char after compareData is not a lettre or number or ' or " or _
                if (!(data[size] > 47 && data[size] < 58) && !(data[size] > 64 && data[size] < 91) && !(data[size] > 96 && data[size] < 123) && data[size] != '_' && data[size] != 39 && data[size] != 34) {
                    return 1;
                }
            } else {
                return 1;
            }
        }
    } else {
        //if data have cher after compareData
        if (strlen(tmp) > 0) {
            if (!(data[size] > 47 && data[size] < 58) && !(data[size] > 64 && data[size] < 91) && !(data[size] > 96 && data[size] < 123) && data[size] != '_' && data[size] != 39 && data[size] != 34) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    return 0;
}
