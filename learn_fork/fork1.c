#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t org_pid = getpid();

    pid_t cur_pid = fork();
    if (cur_pid < 0) {
        printf("Fork fail...\n");
        exit(1);
    }
    else if (cur_pid == 0) { /* child process */
        printf("[%d] I'm child process of %d.\n", getpid(), org_pid);
        exit(0);
    }
    else { /* parent process */
        wait(NULL);
        printf("child process is done.\n");
    }
    printf("[%d] parent process is done.\n", org_pid);
}