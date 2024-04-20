#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void catch_alarm(int sig) {
    printf("Catch SIGALRM(%d)!\n\n", sig);
    exit(0);
}

int main() {
    printf("sleep for 3 seconds.\n\n");
    sleep(3);
    (void) signal(SIGALRM, catch_alarm);

    printf("End!\n\n");
    return 0;
}