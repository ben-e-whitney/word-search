#ifndef CIRCLE_H
#define CIRCLE_H

#include <stddef.h>

#include "linked_list.h"

struct Circle {
    struct LinkedList * const start;
    struct LinkedList * current;
    const size_t length;
};

struct Circle circle_initialize(const size_t);

void circle_finalize(struct Circle * const);

void circle_advance(struct Circle * const);

struct LinkedList * const circle_at(
    struct Circle * const, const enum Direction
);

#endif
