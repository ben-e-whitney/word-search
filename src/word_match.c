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

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS_DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

static unsigned int l_inf_distance(
    const unsigned int row1,
    const unsigned int col1,
    const unsigned int row2,
    const unsigned int col2
) {
    const unsigned int diff_rows = ABS_DIFF(row1, row2);
    const unsigned int diff_cols = ABS_DIFF(col1, col2);
    return MAX(diff_rows, diff_cols);
}

#undef MAX
#undef ABS_DIFF

char word_match_expected(
    const struct WordMatch * const m,
    const unsigned int row,
    const unsigned int col
) {
    const struct WordMatchStarter * const s = m->starter;
    const unsigned int d = l_inf_distance(row, col, m->row, m->col);
    return *(s->start + (ssize_t) d * s->step);
}

unsigned char word_match_finished(
    const struct WordMatch * const m,
    const unsigned int row,
    const unsigned int col
) {
    const struct WordMatchStarter * const s = m->starter;
    const unsigned int d = l_inf_distance(row, col, m->row, m->col);
    return d + 1 == s->length;
}
