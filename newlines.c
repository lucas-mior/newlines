#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) {
    int n;

    if (argc < 2) {
        exit(EXIT_FAILURE);
    }

    if ((n = atoi(argv[1])) <= 0) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i += 1) {
        (void)putchar('\n');
    }
    exit(EXIT_SUCCESS);
}
