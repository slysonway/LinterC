//
// Created by mathi on 2018-11-10.
//

#include "linkedList.h"

/**
 * This function will setup a linked list element
 * @return Linked list element
 */
LinkedList *initList()
{
    LinkedList *newList = malloc(sizeof(LinkedList));

    if(newList == NULL)

        return NULL;

    newList->next       =   NULL;
    newList->previous   =   NULL;
    newList->data       =   NULL;

    return newList;
};

/**
 * This function will add an element a the end of the list
 *
 * @param list The list where we will add, could be null
 * @param data The element who will be stock
 * @return The element created
 */
LinkedList *listAppend(LinkedList *list, void *data)
{
    LinkedList *lastElement = NULL;
    LinkedList *newElement;

    if(list != NULL)

        lastElement =   listGetLast(list);

    newElement = initList();

    if(newElement == NULL)

        return NULL;

    if(lastElement != NULL)
    {
        lastElement->next       =   newElement;
        newElement->previous    =   lastElement;
    }

    newElement->data    =   data;

    return (list != NULL) ? list : newElement;
}

/**
 * This function will add an element a the beginning of the list
 *
 * @param list The list where we will add, could be null
 * @param data The element who will be stock
 * @return The first element of the collection
 */
LinkedList *listPrepend(LinkedList *list, void *data)
{
    LinkedList *firstElement = NULL;
    LinkedList *newElement;

    if(list != NULL)

        firstElement =   listGetFirst(list);

    newElement = initList();

    if(newElement == NULL)

        return NULL;

    if(firstElement != NULL)
    {
        firstElement->previous  =   newElement;
        newElement->next        =   firstElement;
    }

    newElement->data    =   data;

    return newElement;
}

/**
 * This function will remove the element from the list
 * @param list The element to remove
 * @return The first element of the list or null
 */
LinkedList *listRemove(LinkedList *list)
{
    LinkedList *previous;
    LinkedList *next;

    if (list == NULL)

        return NULL;

    previous    = list->previous;
    next        = list->next;

    if(list->previous != NULL)

        list->previous->next = list->next;

    if(list->next != NULL)

        list->next->previous = list->previous;

    free(list);

    if(previous == NULL && next == NULL)

        return NULL;

    if(previous != NULL)

        return listGetFirst(previous);

    return listGetFirst(next);
}

/**
 * Return the first element of the list
 * @param list The list
 * @return First element of the list
 */
LinkedList *listGetFirst(LinkedList *list)
{
    while(list->previous != NULL)

        list = list->previous;

    return list;
}

/**
 * Return the last element of the list
 * @param list
 * @return Last element of the list
 */
LinkedList *listGetLast(LinkedList *list)
{
    while(list->next != NULL)

        list = list->next;

    return list;
}