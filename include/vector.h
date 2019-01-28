#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#include "word_match.h"

struct Vector {
    struct WordMatch * first;
    size_t length;
};

struct Vector vector_initialize(void);

struct WordMatch * const vector_append(
    struct Vector * const, struct WordMatch const
);

void vector_clear(struct Vector * const);

void vector_finalize(struct Vector * const);

#endif
