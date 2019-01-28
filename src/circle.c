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

static struct LinkedList * const circle_at_n_hence(
    struct Circle * const c, const size_t n
) {
    return c->length ?
        c->start + ((c->current - c->start) + n) % c->length :
        NULL;
}

void circle_advance(struct Circle * const c) {
    c->current = circle_at_n_hence(c, 1);
}

/*This assumes that `c->length` is sufficiently large and it doesn't do any*/
/*error checking about `heading`.*/
struct LinkedList * const circle_at(
    struct Circle * const c, enum Direction heading
) {
    size_t n = 0;
    switch (heading) {
        case (SOUTHWEST):
            n = c->length - 3;
            break;
        case (SOUTH):
            n = c->length - 2;
            break;
        case (SOUTHEAST):
            n = c->length - 1;
            break;
        case (EAST):
            n = 1;
            break;
    }
    return circle_at_n_hence(c, n);
}
