#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "word_match.h"

struct LinkedListElement {
    struct LinkedListElement *prev;
    const struct WordMatch * match;
    struct LinkedListElement *next;
};

struct LinkedList {
    struct LinkedListElement * first;
    struct LinkedListElement * last;
};

void linked_list_add(struct LinkedList * const, struct WordMatch * const);

void linked_list_remove(
    struct LinkedList * const,
    struct LinkedListElement * const
);

#endif
