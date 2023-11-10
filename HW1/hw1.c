#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "usage: test <val1> <val2> <val3>\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        printf("%p\n", &argv[i]);
    }
    return 0;
}
