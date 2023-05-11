#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *progname;
void usage(FILE *stream) {
    fprintf(stream, "usage: %s <n>\n"
                    "<n> : number of new lines to print\n", progname);
    exit((int) (stream != stdout));
}

int main(int argc, char *argv[]) {
    progname = argv[0];
    int n = 0;
    int i = 0;

    if (argc <= 1 || argc >= 3)
        usage(stderr);

    if ((n = atoi(argv[1])) < 0)
        usage(stderr);

    while (++i <= n)
        (void) puts("");
    return 0;
}
