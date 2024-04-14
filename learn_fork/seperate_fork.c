#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t cur = fork();

    if (cur < 0) {
        printf("fork error!\n");
        exit(1);
    }
    else if (cur == 0) { /* child */
        execlp("./hello.out", "hello", "1", "3", NULL);
    }
    else { /* parent */
        wait(NULL);
        printf("child complete.\n");
    }
    printf("parent complete.\n");
}