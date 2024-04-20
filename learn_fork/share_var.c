#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int a = 1;
float b = 1.3;
double c = 873.294;
long long d = 9999999999;

int main() {

    printf("== Original Value ======\n");
    printf("a: %d\n", a);
    printf("b: %f\n", b);
    printf("c: %lf\n", c);
    printf("d: %lld\n\n", d);

    pid_t pid = fork();
    if (pid < 0)  {
        printf("fork error!");
        exit(1);
    }
    else if (pid == 0) { /* child section */

        // change var at child process
        a += 3;
        b *= 3.45;
        c -= 73.5;
        d /= 3;

        printf("== Child Process ======\n");
        printf("a: %d\n", a);
        printf("b: %f\n", b);
        printf("c: %lf\n", c);
        printf("d: %lld\n\n", d);    

        exit(0);
    }
    else {  /* parent section */
        wait(NULL);

        printf("== Parent Process ======\n");
        printf("a: %d\n", a);
        printf("b: %f\n", b);
        printf("c: %lf\n", c);
        printf("d: %lld\n\n", d);        
    }
    return 0;
}