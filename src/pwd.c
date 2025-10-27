#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    size_t length = 16;
    char *cwd = malloc(sizeof(char) * length);

    if (cwd == NULL) {
        perror("malloc");
        return 1;
    }

    while(getcwd(cwd, length) == NULL) {
        if (errno != ERANGE) {
            perror("getcwd");
            return errno;
        }

        length *= 2;
        char *tmp = realloc(cwd, length);

        if (tmp == NULL) {
            perror("realloc");
            free(cwd);
            return 1;
        }

        cwd = tmp;
    }

    printf("%s\n", cwd);
    free(cwd);

    return 0;
}
