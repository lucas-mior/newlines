#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *progname;
static inline void usage(FILE *stream) {
    fprintf(stream, "usage: %s [ <n> | h ]\n"
                    "<n> : number of new lines to print\n"
                    " h  : prints this help message to stdout\n", progname);
    exit(0);
}

int main(int argc, char *argv[]) {
    progname = argv[0];
    int n = 0;
    int i = 0;

    if (argc <= 1 || argc >= 3)
        usage(stderr);

    switch (argv[1][0]) {
        case 'h':
            usage(stdout);
            break;
        default:
            n = atoi(argv[1]);
            break;
    }

    if (n < 0)
        usage(stderr);

    while (++i <= n)
        (void) puts("");
    return 0;
}
