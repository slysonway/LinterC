//
// Created by mathi on 2018-11-10.
//

#include "fileFunction.h"

/**
 * This function will read recursively all c file in a directory
 * @param filename The path to open
 * @param recursive Il the research is recursive
 * @param param The param for research
 */
void readAllDirectory(char *filename, int recursive, ConfElement *param)
{
    char *temp;
    DIR *dir;
    char buffer[500] = {0};
    struct dirent *directoryInfo;
    int find = 0;
    LinkedList *i;

    // Start, by opening the directory
    dir = opendir(filename);

    // If the directory have been correctly open
    if(dir)
    {
        // While the function find a file or a directory
        while((directoryInfo = readdir(dir)) != NULL)
        {
            // If the element is a file
            if(directoryInfo->d_type == DT_REG)
            {
                // We check for  extension
                if((temp = strrchr(directoryInfo->d_name, '.')) != NULL && strcmp(temp  + 1, "c") == 0)
                {
                    find = 0;

                    // Then check if this file is not in excluded file list
                    for(i = param->excludedFiles; i != NULL; i = i->next)
                    {
                        if(strcmp(i->data, directoryInfo->d_name) == 0)
                        {
                            find = 1;
                            break;
                        }
                    }

                    // If the file is not in excluded file
                    if(find == 0)
                    {
                        // Prepare the full path for fopen
                        strcpy(buffer, filename);
                        strcat(buffer, "/");
                        strcat(buffer, directoryInfo->d_name);

                        // And inspect the file
                        exploreFile(buffer, param);
                    }
                }
            }
            // If the element is a directory
            // And the user ask for recursive research
            else if(directoryInfo->d_type == DT_DIR && recursive == 1 && strcmp(directoryInfo->d_name, ".") != 0 && strcmp(directoryInfo->d_name, "..") != 0)
            {
                // Prepare the new path
                strcpy(buffer, filename);
                strcat(buffer, "/");
                strcat(buffer, directoryInfo->d_name);

                // Restart the same function
                // On new directory
                readAllDirectory(buffer, recursive, param);
            }
        }

        // At the end close the directory
        closedir(dir);
    }
}

/**
 *
 * @param fileName
 * @param param
 */
void exploreFile(char * fileName, ConfElement *param)
{
    // Starting, by opening the file
    FILE *f   =   fopen(fileName, "r");
    char buffer[2000];
    int line = 1;
    int error = 0;
    char errorMessage[600] = {'\0'};
    int fileError = 0;

    printf("Fichier %s:\n", fileName);

    // If the file have been correctly open
    if(f != NULL)
    {
        // We read the file line by line
        while(fgets(buffer, 2000, f) != NULL)
        {
            error = 0;

            for(LinkedList *i = param->rules; i != NULL; i = i->next)
            {
                //buffer[strlen(buffer)] = '\0';

                ((RulesElement *)i->data)->initFunction(line, buffer, ((RulesElement *)i->data), &error, errorMessage);
                if (line > 110) {
                }
            }

            if(error != 0)
            {
                printf("    Line n°%d:\n", line);
                printf("%s", errorMessage);

                fileError = 1;

                strcpy(errorMessage, "");
            }

            line++;
        }
    }

    if(fileError == 0)

        printf("    No error found\n");

    printf("\n");
}
