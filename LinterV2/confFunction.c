//
// Created by mathi on 2018-11-10.
//

#include "confFunction.h"
#include "allRules.h"

/**
 * This function will initialise the conf structure
 * With the default param
 * @return The conf structure set
 */
ConfElement *initGetParam()
{
    ConfElement *newConf    =   malloc(sizeof(ConfElement));

    getParam("default.lconf", newConf);

    return newConf;
}

/**
 * This function will read a conf file and set the param
 * @param file The conf file to read
 * @param newConf The structure to put the data
 * @return The structure set
 */
ConfElement *getParam(char *file, ConfElement *newConf)
{
    // Starting, by opening the file
    FILE *f                 =   fopen(file, "r");
    char tempTitle[200];
    char buffer[2000];
    char temp[200];
    char *tempChar;
    int lineCountTitle = 0;

    // If no conf structure pass
    // Stop the execution
    if(newConf == NULL)

        return NULL;

    // Initialize will null all the param
    newConf->rules          =   NULL;
    newConf->excludedFiles  =   NULL;

    // If the file have been correctly open
    if(f != NULL)
    {
        // We read the file line by line
        while(fgets(buffer, 2000, f) != NULL)
        {
            // If the line start with an equal
            // It's a main key
            if(buffer[0] == '=')
            {
                // strncpy will not put the \0 at the end of the string
                // So put \0 on all temp title
                // And then put the buffer without the \n
                memset(tempTitle, '\0', sizeof(tempTitle));
                strncpy(tempTitle, buffer + 1, strlen(buffer) - 2);

                lineCountTitle = 0;

                continue;
            }

            // If the current main key is extends
            // And if we are the line after the main key definition
            if(strcmp(tempTitle, "extends") == 0 && strlen(buffer) > 2 && lineCountTitle == 0)
            {
                // Get the file to open
                memset(temp, '\0', sizeof(temp));
                strncpy(temp, buffer, strlen(buffer) - 1);

                // Get the param of parent file
                getParam(temp, newConf);
            }

            // If the current key is exclude
            // Save the file to exclude
            if(strcmp(tempTitle, "excludedFiles") == 0 && buffer[0] == '-' && strlen(buffer) > 2)
            {
                //puts(buffer);
                tempChar = malloc(strlen(buffer));

                memset(tempChar, '\0', sizeof(tempChar));
                strncpy(tempChar, buffer, strlen(buffer) - 1);

                newConf->excludedFiles = listPrepend(newConf->excludedFiles, tempChar + 2);
            }

            // If the current key is rules
            // Save the rules to rules list
            if(strcmp(tempTitle, "rules") == 0 && buffer[0] == '-' && strlen(buffer) > 2)

                newConf->rules = addRules(newConf->rules, buffer + 2);

            lineCountTitle++;
        }
    }
    else
    {
        puts("Can't open the file");
    }

    // return the conf set
    return newConf;
}

/**
 * This function will add rules to conf list
 * @param rulesList The rules list
 * @param rulesDetail The current rule detail
 * @return The rules list update
 */
LinkedList *addRules(LinkedList *rulesList, char *rulesDetail)
{
    RulesElement *rulesParam;
    LinkedList *i = NULL;
    LinkedList *same = NULL;
    char *temp;

    // Start by create a new rule struct
    rulesParam  =   malloc(sizeof(RulesElement));

    // If the malloc fail
    // Return the list
    if(rulesParam == NULL)

        return rulesList;

    // Initialise the param
    rulesParam->number      =   0;
    rulesParam->baseData    =   NULL;
    rulesParam->temp        =   0;

    // Continuing by set the different type of rule param
    if(strstr(rulesDetail, "array-bracket-eol"))
    {
        strcpy(rulesParam->rulesName, "array-bracket-eol");
        rulesParam->initFunction    =   arrayBracketEol;
    }

    if(strstr(rulesDetail, "operators-spacing"))
    {
        strcpy(rulesParam->rulesName, "operators-spacing");
        rulesParam->initFunction    =   operatorsSpacing;
    }

    if(strstr(rulesDetail, "variable-assignment-type"))
    {
        strcpy(rulesParam->rulesName, "variable-assignment-type");
        rulesParam->initFunction    =   variableAssignmentType;
    }

    if(strstr(rulesDetail, "indent"))
    {
        strcpy(rulesParam->rulesName, "indent");
        rulesParam->initFunction    =   indent;
    }
    
    if(strstr(rulesDetail, "no-multi-declaration")) {
        strcpy(rulesParam->rulesName, "no-multi-declaration");
        rulesParam->initFunction = noMultiDeclaration;
    }
    
    if(strstr(rulesDetail, "unused-variable")) {
        strcpy(rulesParam->rulesName, "unused-variable");
        rulesParam->initFunction = unusedVariable;
    }
    
    if(strstr(rulesDetail, "undeclared-variable")) {
        strcpy(rulesParam->rulesName, "undeclared-variable");
        rulesParam->initFunction = undeclaredVariable;
    }
    
    if(strstr(rulesDetail, "no-prototype")) {
        strcpy(rulesParam->rulesName, "no-prototype");
        rulesParam->initFunction = noPrototype;
    }
    
    if(strstr(rulesDetail, "unused-function")) {
        strcpy(rulesParam->rulesName, "unused-function");
        rulesParam->initFunction = unusedFunction;
    }


    if(rulesList != NULL)
    {
        for(i = rulesList; i != NULL; i = i->next)
        {
            if(strcmp(rulesParam->rulesName, ((RulesElement *)i->data)->rulesName) == 0)
            {
                same = i;
                break;
            }
        }
    }

    if(strstr(rulesDetail, "off") != NULL)
    {
        free(rulesParam);

        if(same != NULL)

            return listRemove(same);


        return rulesList;
    }

    if(strstr(rulesDetail, "on") == NULL)
    {
        rulesParam->number = (int) strtol(strchr(rulesDetail, '=') + 1, &temp, 10);
    }

    if(same != NULL)
    {
        ((RulesElement *)same->data)->number = rulesParam->number;

        free(rulesParam);

        return rulesList;
    }

    rulesList = listPrepend(rulesList, rulesParam);

    return rulesList;
}

