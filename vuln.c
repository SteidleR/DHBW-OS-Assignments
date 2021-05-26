#include <string.h>
#include <stdio.h>

void do_something(char *string) {
    char buffer[500];
    strcpy(buffer, string);
}

void main (int argc, char* argv[]) {
    printf("Calling vulnerable function!\n");
    do_something(argv[1]);
}