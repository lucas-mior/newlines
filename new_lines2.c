#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int n;
    char lines[128];
    memset(lines, '\n', 127);

    if (argc < 2)
        return 0;

    if ((n = atoi(argv[1])) <= 0)
        return 0;

    lines[n] = '\0';
    puts(lines);
    return 0;
}
