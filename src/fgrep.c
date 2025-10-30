/*
 * fgrep
 *
 * Usage: fgrep [-A num] [-B num] <pattern> <file>
*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Line line_t;

typedef struct Line {
    char *buf;
    int len;
    line_t *prev;
    line_t *next;
} line_t;

line_t *file_read(char *file_name) {
    char *buf = NULL;
    size_t len = 0;

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "fgrep: %s: %s\n", file_name, strerror(errno));
        return NULL;
    }

    line_t *first_line = NULL;
    line_t *current = NULL;

    while (getline(&buf, &len, file) != -1) {
        char *line_buf = malloc(sizeof(char) * (len + 1));
        if (line_buf == NULL) {
            fprintf(stderr, "fgrep: line_buf malloc failed");
            return NULL;
        }

        strcpy(line_buf, buf);

        line_t *line = malloc(sizeof(line_t));
        if (line == NULL) {
            free(line_buf);
            fprintf(stderr, "fgrep: line malloc failed");
            return NULL;
        }
        
        line->buf = line_buf;
        line->len = len;

        if (first_line == NULL) {
            first_line = line;
            current = line;
        } else {
            current->next = line;
            line->prev = current;
            current = line;
        }
    }

    fclose(file);
    return first_line;
}

void file_print_matches(char *pattern, line_t *start_line, int leading_ctx, int trailing_ctx)
{
    if (start_line == NULL) {
        return;
    }

    line_t* current = start_line;
    line_t* last_out = NULL;

    while (current != NULL) {
        if (strstr(current->buf, pattern)) {
            line_t *match = current;

            if (match->prev != NULL && last_out != NULL && match->prev != last_out) {
                printf("--\n");
            }

            for (int i = 0; i < leading_ctx; i++) {
                if (current->prev == last_out) {
                    break;
                }
                current = current->prev;
            }

            while (current != match) {
                printf("%s", current->buf);
                current = current->next;
            }

            printf("%s", current->buf);

            for (int i = 0; i < trailing_ctx; i++) {
                current = current->next;
                if (current == NULL) {
                    break;
                }
                printf("%s", current->buf);
            }

            last_out = current;
        }
        
        
        if (current != NULL) {
            current = current->next;
        }
    } 
}

int main(int argc, char *argv[])
{
    int trailing_ctx = 0;
    int leading_ctx = 0;
    char *pattern = NULL;
    char *file_name = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "A:B:")) != -1) {
        switch(opt) {
            case 'A':
                trailing_ctx = atoi(optarg);
                break;
            case 'B':
                leading_ctx = atoi(optarg);
                break;
        }
    }

    if (optind < argc - 1) {
        pattern = argv[optind++];
        file_name = argv[optind++];
    }

    if (pattern == NULL || file_name == NULL) {
        printf("Usage: fgrep [-A num] [-B num] <pattern> <file>\n");
        return 1;
    }

    line_t *first_line = file_read(file_name);
    if (first_line == NULL) {
        return 1;
    }

    file_print_matches(pattern, first_line, leading_ctx, trailing_ctx);

    return 0;
}
