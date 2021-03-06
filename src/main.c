#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "circle.h"
#include "direction.h"
#include "word_match.h"

#define SWAP(T, a, b) do {T tmp = a; a = b; b = tmp;} while (0)

void report_match(const struct WordMatch * const m) {
    const struct WordMatchStarter * const s = m->starter;
    signed char row_step = 0;
    signed char col_step = 0;
    switch (m->heading) {
        case (SOUTHWEST):
            row_step = 1;
            col_step = -1;
            break;
        case (SOUTH):
            row_step = 1;
            break;
        case (SOUTHEAST):
            row_step = 1;
            col_step = 1;
            break;
        case (EAST):
            col_step = 1;
            break;
    }
    unsigned int row_start = m->row;
    unsigned int col_start = m->col;
    unsigned int row_end = row_start + row_step * (ssize_t) (s->length - 1);
    unsigned int col_end = col_start + col_step * (ssize_t) (s->length - 1);
    if (s->step == -1) {
        SWAP(unsigned int, row_start, row_end);
        SWAP(unsigned int, col_start, col_end);
    }
    fprintf(
        stdout,
        "%s from (%u, %u) to (%u, %u)\n",
        m->starter->word, row_start, col_start, row_end, col_end
    );
}

int main(void) {
    char *line = NULL;
    {
        size_t n = 0;
        ssize_t nbytes = getline(&line, &n, stdin);
        if (nbytes == -1) {
            fprintf(stderr, "failed to read size line\n");
            exit(1);
        }
    }
    unsigned int nrows;
    unsigned int ncols;
    unsigned int nwords;
    {
        const int nvar = sscanf(line, "%u %u %u\n", &nrows, &ncols, &nwords);
        if (nvar != 3) {
            fprintf(
                    stderr,
                    "failed to parse size line\n"
                    "expected format: [nrows] [ncols] [nwords]"
                   );
            exit(1);
        }
    }
    free(line);

    struct WordMatchStarter * const starters = malloc(
        2 * nwords * sizeof(*starters)
    );
    unsigned int letter_counts[26];
    for (unsigned int i = 0; i < 26; ++i) {
        letter_counts[i] = 0;
    }

    char ** const words = malloc(nwords * sizeof(*words));
    for (unsigned int i = 0; i < nwords; ++i) {
        words[i] = NULL;
        size_t n = 0;
        const ssize_t nbytes = getline(words + i, &n, stdin);
        if (nbytes == -1) {
            fprintf(stderr, "failed to read word %u\n", i);
            exit(1);
        }
        char * const p = words[i];
        unsigned int length = 0;
        for (unsigned int j = 0; j < n; ++j) {
            const char c = p[j];
            if (c == '\n') {
                p[j] = '\0';
                length = j;
                break;
            } else if (!isupper(c)) {
                fprintf(
                    stderr, "word %u: '%c' is not uppercase\n", i, c
                );
                exit(1);
            }
        }
        if (!length) {
            fprintf(stderr, "word %u: word is empty\n", i);
            exit(1);
        }
        ++letter_counts[p[0] - 'A'];
        ++letter_counts[p[length - 1] - 'A'];
    }

    struct WordMatchStarter *letter_starts[26];
    struct WordMatchStarter *p = starters;
    for (unsigned int i = 0; i < 26; ++i) {
        letter_starts[i] = p;
        p += letter_counts[i];
        /*`letter_counts[i]` will now be used to tell us how many
         * `struct WordMatchStarter`s have been written for each letter.*/
        letter_counts[i] = 0;
    }

    for (unsigned int i = 0; i < nwords; ++i) {
        const char *start;
        unsigned int j;
        struct WordMatchStarter * p;
        const char * const word = words[i];
        const size_t length = strlen(word);

        start = word;
        j = *start - 'A';
        p = letter_starts[j] + letter_counts[j];
        word_match_starter_initialize(p, word, length, start, 1);
        ++letter_counts[j];

        start = word + (length - 1);
        j = *start - 'A';
        p = letter_starts[j] + letter_counts[j];
        word_match_starter_initialize(p, word, length, start, -1);
        ++letter_counts[j];
    }

    struct Circle todo = circle_initialize(ncols + 2);
    for (unsigned int row = 0; row < nrows; ++row) {
        int c;
        c = fgetc(stdin);
        if (c == EOF) {
            fprintf(stderr, "too few rows (read %u)\n", row);
            exit(1);
        }
        int d = ungetc(c, stdin);
        if (d == EOF) {
            fprintf(stderr, "failed to push back character '%c'\n", c);
            exit(1);
        }
        for (unsigned int col = 0; col < ncols; ++col) {
            c = fgetc(stdin);
            if (c == EOF || c == '\n') {
                fprintf(
                    stderr, "row %u: too few columns (read %u)\n", row, col
                );
                exit(1);
            } else if (!isupper(c)) {
                fprintf(
                    stderr,
                    "row %u, col %u: '%c' is not uppercase\n",
                    row, col, c
                );
                exit(1);
            }
            const unsigned int j = c - 'A';
            const unsigned int K = letter_counts[j];
            struct Vector * neighboring_todos[NUMBER_OF_DIRECTIONS];
            for (unsigned char i = 0; i < NUMBER_OF_DIRECTIONS; ++i) {
                neighboring_todos[i] = circle_at(&todo, (enum Direction) i);
            }
            for (unsigned int k = 0; k < K; ++k) {
                const struct WordMatchStarter *p = letter_starts[j] + k;
                unsigned char room[NUMBER_OF_DIRECTIONS];
                room[SOUTH] = row + p->length <= nrows;
                room[EAST] = col + p->length <= ncols;
                room[SOUTHEAST] = room[SOUTH] && room[EAST];
                /*Second term would be `room[WEST]`.*/
                room[SOUTHWEST] = room[SOUTH] && (1 + col >= p->length);
                for (unsigned char i = 0; i < NUMBER_OF_DIRECTIONS; ++i) {
                    if (room[i]) {
                        vector_append(
                            neighboring_todos[i],
                            (struct WordMatch) {
                                .starter = p,
                                .row = row,
                                .col = col,
                                .heading = (enum Direction) i
                            }
                        );
                    }
                }
            }
            struct Vector * const todo_he = todo.current;
            for (size_t i = 0; i < todo_he->length; ++i) {
                const struct WordMatch * const m = todo_he->first + i;
                const char expected = word_match_expected(m, row, col);
                if (c == expected) {
                    if (word_match_finished(m, row, col)) {
                        report_match(m);
                    } else {
                        vector_append(neighboring_todos[m->heading], *m);
                    }
                }
            }
            vector_clear(todo_he);
            circle_advance(&todo);
        }
        c = fgetc(stdin);
        if (c == EOF) {
            fprintf(stderr, "row %u: row doesn't end in a newline\n", row);
            exit(1);
        } else if (c != '\n') {
            fprintf(stderr, "row %u: too many columns\n", row);
            exit(1);
        }
    }

    circle_finalize(&todo);
    for (unsigned int i = 0; i < nwords; ++i) {
        free(words[i]);
    }
    free(words);
    free(starters);
    return 0;
}
