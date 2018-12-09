//
// Created by mathi on 2018-11-10.
//

#ifndef LINTERPROJECT_LINKEDLIST_H
#define LINTERPROJECT_LINKEDLIST_H

#include <stdlib.h>

typedef struct LinkedList
{
    struct LinkedList *previous;
    struct LinkedList *next;
    void *data;

} LinkedList;

LinkedList *initList(void);

LinkedList *listAppend(LinkedList *list, void *data);

LinkedList *listPrepend(LinkedList *list, void *data);

LinkedList *listRemove(LinkedList *list);

LinkedList *listGetFirst(LinkedList *list);

LinkedList *listGetLast(LinkedList *list);

#endif //LINTERPROJECT_LINKEDLIST_H
