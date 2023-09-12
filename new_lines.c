#include <stdio.h>
#include <stdlib.h>

static void usage(FILE *) __attribute__((noreturn));

int main(int argc, char *argv[]) {
    int n = 0;
    int i = 0;

    if (argc <= 1 || argc >= 3)
        usage(stderr);

    if ((n = atoi(argv[1])) < 0)
        usage(stderr);

    while (i < n) {
        i += 1;
        (void) putchar('\n');
    }
    return 0;
}

void usage(FILE *stream) {
    fprintf(stream, "usage: new_lines <n>\n"
                    "<n> : number of new lines to print\n");
    exit((int) (stream != stdout));
}
