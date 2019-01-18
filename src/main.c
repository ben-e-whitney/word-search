#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE_LINE_LENGTH 100

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
        int nvar = sscanf(line, "%u %u %u\n", &nrows, &ncols, &nwords);
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

    char **words = (char **) malloc(nwords * sizeof(char *));
    for (unsigned int i = 0; i < nwords; ++i) {
        words[i] = NULL;
        size_t n = 0;
        ssize_t nbytes = getline(words + i, &n, stdin);
        if (nbytes == -1) {
            fprintf(stderr, "failed to read word %u\n", i);
            exit(1);
        }
        char *p = words[i];
        for (unsigned int j = 0; j < n; ++j) {
            if (p[j] == '\n') {
                p[j] = '\0';
                break;
            }
        }
    }

    for (unsigned int i = 0; i < nrows; ++i) {
        int c;
        c = fgetc(stdin);
        if (c == EOF) {
            fprintf(stderr, "too few rows (read %u)\n", i);
            exit(1);
        }
        int d = ungetc(c, stdin);
        if (d == EOF) {
            fprintf(stderr, "failed to push back character '%c'\n", c);
            exit(1);
        }
        for (unsigned int j = 0; j < ncols; ++j) {
            c = fgetc(stdin);
            if (c == EOF || c == '\n') {
                fprintf(stderr, "row %u: too few columns (read %u)\n", i, j);
                exit(1);
            }
        }
        c = fgetc(stdin);
        if (c == EOF) {
            fprintf(stderr, "row %u: row doesn't end in a newline\n", i);
            exit(1);
        } else if (c != '\n') {
            fprintf(stderr, "row %u: too many columns\n", i);
            exit(1);
        }
    }

    for (unsigned int i = 0; i < nwords; ++i) {
        free(words[i]);
    }
    free(words);
}
