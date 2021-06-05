#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    int fd, rtn;
    fd = open("/dev/memory", O_RDWR);
    if (fd == -1) {
            perror("open /dev/memory");
            exit(EXIT_FAILURE);
    }

    printf("Writing to /dev/memory\n");
    char c[512];
    strcpy(c, "hello World! Are you okay?");
    write(fd, &c, 512);

    printf("Reading from /dev/memory: \n");
    read(fd, &c, 1);
    printf("%s", c);
    exit(EXIT_SUCCESS);
}