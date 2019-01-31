#ifndef WORD_MATCH_H
#define WORD_MATCH_H

#include <stdlib.h>

#include "direction.h"

struct WordMatchStarter {
    const char *word;
    size_t length;
    const char *start;
    signed char step;
};

void word_match_starter_initialize(
    struct WordMatchStarter * const p,
    const char * const word,
    const size_t length,
    const char * const start,
    const signed char step
    );

struct WordMatch {
    const struct WordMatchStarter * starter;
    unsigned int row;
    unsigned int col;
    enum Direction heading;
};

char word_match_expected(
    const struct WordMatch * const, const unsigned int, const unsigned int
);

unsigned char word_match_finished(
    const struct WordMatch * const, const unsigned int, const unsigned int
);

#endif
