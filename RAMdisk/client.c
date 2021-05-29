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
                perror("open /dev/chardev");
                exit(EXIT_FAILURE);
        }

        printf("Writing to /dev/memory\n");
        char c[16];
        strcpy(c, "hello World!");
        write(fd, &c, 12);

        printf("Reading from /dev/memory: \n");
        read(fd, &c, 1);
        printf("%s", c);
        exit(EXIT_SUCCESS);
}