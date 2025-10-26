#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("\n");
        return 0;
    }

    bool flag_n = (strcmp(argv[1], "-n") == 0);

    if (flag_n && argc == 2) {
        return 0;
    }

    int i = flag_n ? 2 : 1;
    for (; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }

    if (!flag_n) {
        printf("\n");
    }

    return 0;
}
