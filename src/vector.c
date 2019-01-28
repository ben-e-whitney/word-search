#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

struct Vector vector_initialize(void) {
    return (struct Vector) {.first = NULL, .length = 0};
}

struct WordMatch * const vector_append(
    struct Vector * const v, struct WordMatch const m
) {
    struct WordMatch * const p = realloc(
        v->first, (1 + v->length) * sizeof(*p)
    );
    if (p == NULL) {
        fprintf(stderr, "failed to allocate memory for vector append\n");
        return NULL;
    }
    v->first = p;
    struct WordMatch *q = v->first + v->length;
    *q = m;
    ++v->length;
    return q;
}

void vector_clear(struct Vector * const v) {
    v->first = realloc(v->first, 0);
    v->length = 0;
}

void vector_finalize(struct Vector * const v) {
    free(v->first);
    v->first = NULL;
    v->length = 0;
}
