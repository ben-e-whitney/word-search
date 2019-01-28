#include <stdio.h>

#include "circle.h"

struct Circle circle_initialize(const size_t length) {
    struct LinkedList *p = malloc(length * sizeof(*p));
    if (p == NULL) {
        fprintf(stderr, "failed to allocate memory for circle\n");
    }
    for (size_t i = 0; i < length; ++i) {
        struct LinkedList *q = p + i;
        q->last = q->first = NULL;
    }
    return (struct Circle) {.start = p, .current = p, .length = length};
}

void circle_finalize(struct Circle * const c) {
    free(c->start);
    c->current = NULL;
}

void circle_advance(struct Circle * const c) {
    c->current = circle_at(c, 1);
}

struct LinkedList * const circle_at(struct Circle * const c, const size_t i) {
    return c->length ?
        c->start + ((c->current - c->start) + i) % c->length :
        NULL;
}
