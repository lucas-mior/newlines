#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static char *progname;
static void usage(FILE *) __attribute__((noreturn));

int main(int argc, char *argv[]) {
    int n = 0;
    /* int i = 0; */
    char buffer[256];
    progname = argv[0];

    if (argc <= 1 || argc >= 3)
        usage(stderr);

    if ((n = atoi(argv[1])) < 0)
        usage(stderr);

    n = n > 256 ? 256 : n;
    memset(buffer, '\n', n);
	write(STDOUT_FILENO, buffer, n);

    /* while (++i <= n) */
    /*     (void) puts(""); */
    return 0;
}

void usage(FILE *stream) {
    fprintf(stream, "usage: %s <n>\n"
                    "<n> : number of new lines to print\n", progname);
    exit((int) (stream != stdout));
}
