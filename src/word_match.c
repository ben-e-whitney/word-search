#include "word_match.h"

void initialize_word_match_starter(
    struct WordMatchStarter * const p,
    const char * const word,
    const size_t length,
    const char * const start,
    const signed char step
    ) {
    p->word = word;
    p->length = length;
    p->start = start;
    p->step = step;
}

void initialize_word_match(
    struct WordMatch * const p,
    const struct WordMatchStarter * const starter,
    const unsigned int row,
    const unsigned int col,
    const enum Direction heading
) {
    p->starter = starter;
    p->row = row;
    p->col = col;
    p->heading = heading;
}
