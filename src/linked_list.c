#include <stdio.h>

#include "linked_list.h"
#include "word_match.h"

void add_element(struct LinkedList * const list, struct WordMatch * const m) {
    if (list == NULL) {
        fprintf(stderr, "attempt to add to null list\n");
        exit(1);
    }
    struct LinkedListElement * const l = malloc(sizeof *l);
    if (l == NULL) {
        fprintf(stderr, "failure to allocate list element memory\n");
        exit(1);
    }
    l->prev = list->last;
    l->match = m;
    l->next = NULL;
    if (list->first == NULL) {
        list->first = l;
    }
    if (list->last != NULL) {
        list->last->next = l;
    }
    list->last = l;
}

void remove_element(
        struct LinkedList * const list,
        struct LinkedListElement * const element
) {
    if (list == NULL) {
        fprintf(stderr, "attempt to remove from null list\n");
        exit(1);
    }
    if (element == NULL) {
        fprintf(stderr, "attempt to remove null element\n");
        exit(1);
    }
    struct LinkedListElement * const p = element->prev;
    struct LinkedListElement * const n = element->next;
    if (p != NULL) {
        p->next = n;
    } else {
        list->first = n;
    }
    if (n != NULL) {
        n->prev = p;
    } else {
        list->last = p;
    }
    free(element);
}
