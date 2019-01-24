#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "linked_list.h"
#include "word_match.h"

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

    struct WordMatchStarter *starters = malloc(2 * nwords * sizeof(*starters));
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
        initialize_word_match_starter(p, word, length, start, 1);
        ++letter_counts[j];

        start = word + (length - 1);
        j = *start - 'A';
        p = letter_starts[j] + letter_counts[j];
        initialize_word_match_starter(p, word, length, start, -1);
        ++letter_counts[j];
    }

    struct LinkedList *matches = malloc(sizeof(*matches));
    matches->first = NULL;
    matches->last = NULL;
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
            const struct WordMatchStarter * const p = letter_starts[j];
            const unsigned int K = letter_counts[j];
            for (unsigned int k = 0; k < K; ++k) {
                const struct WordMatchStarter *p = letter_starts[j] + k;
                unsigned char room_west = 1 + col >= p->length;
                unsigned char room_south = row + p->length <= nrows;
                unsigned char room_east = col + p->length <= ncols;
                if (room_west && room_south) {
                    struct WordMatch *m = malloc(sizeof(*m));
                    initialize_word_match(m, p, row, col, SOUTHWEST);
                    add_element(matches, m);
                }
                if (room_south) {
                    struct WordMatch *m = malloc(sizeof(*m));
                    initialize_word_match(m, p, row, col, SOUTH);
                    add_element(matches, m);
                }
                if (room_south && room_east) {
                    struct WordMatch *m = malloc(sizeof(*m));
                    initialize_word_match(m, p, row, col, SOUTHEAST);
                    add_element(matches, m);
                }
                if (room_east) {
                    struct WordMatch *m = malloc(sizeof(*m));
                    initialize_word_match(m, p, row, col, EAST);
                    add_element(matches, m);
                }
            }
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
    free(matches);

    for (unsigned int i = 0; i < nwords; ++i) {
        free(words[i]);
    }
    free(words);
    return 0;
}
