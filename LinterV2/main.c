#include <stdio.h>
#include "confFunction.h"
#include "fileFunction.h"

int main()
{
    ConfElement *confParam = initGetParam();

    readAllDirectory(".", 1, confParam);
}