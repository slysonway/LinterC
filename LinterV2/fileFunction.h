//
// Created by mathi on 2018-11-10.
//

#ifndef LINTERPROJECT_FILEFUNCTION_H
#define LINTERPROJECT_FILEFUNCTION_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "linkedList.h"
#include "confFunction.h"

void readAllDirectory(char *filename, int recursive, ConfElement *param);

void exploreFile(char * fileName, ConfElement *param);

#endif //LINTERPROJECT_FILEFUNCTION_H
