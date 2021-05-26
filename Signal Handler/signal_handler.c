#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signum) {
    switch (signum)
    {
    case 2:
        printf("User tried to stoppe program with CTRL+C\n");
        break;
    case 9:
        printf("Program has catched some 'kill' command!\n");
        exit(0);
    case 15:
        printf("CTRL+Z was catched!\n");
        exit(0);
    
    default:
        exit(0);
        break;
    }
}

void main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGKILL, handle_signal); // not working: KILL and STOP cannot been catched
    for (int n=1;;n++) {
        sleep(1);
    }
}