#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    if (pid < 0) {
        printf("Error!\n");
        return 0;
    }
    else if (pid == 0) {
        printf("Hello! I'm child process!\n");

        int rslt = 0;
        for (int i = 1; i <= 100; ++i) rslt += i;
        printf("1 + 2 + ... + 100 = %d\n", rslt);

        printf("child Process is Terminated.\n");
        exit(0);
    }
    else {
        wait(NULL);
        printf("Hello! I'm parent process!\n");

        printf("Program is Terminated.\n");
        return 0;
    }

    
}