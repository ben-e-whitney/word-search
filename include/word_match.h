#ifndef WORD_MATCH_H
#define WORD_MATCH_H

#include <stdlib.h>

struct WordMatchStarter {
    const char *word;
    size_t length;
    const char *start;
    signed char step;
};

void initialize_word_match_starter(
    struct WordMatchStarter * const p,
    const char * const word,
    const size_t length,
    const char * const start,
    const signed char step
    );

enum direction {SOUTHWEST, SOUTH, SOUTHEAST, EAST};

struct WordMatch {
    const struct WordMatchStarter * starter;
    unsigned int row;
    unsigned int col;
    enum direction heading;
};

void initialize_word_match(
    struct WordMatch * const p,
    const struct WordMatchStarter * const starter,
    const unsigned int row,
    const unsigned int col,
    const enum direction heading
);

#endif