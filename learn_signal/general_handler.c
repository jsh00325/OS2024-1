#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

typedef void (*sighandler_t) (int);
sighandler_t signal(int sig, sighandler_t pFunc);

void myHandler(int sig) {
    printf("\nI got signal %d.\n", sig);
    (void) signal(sig, SIG_DFL);
}

int main() {
    sighandler_t prvRet;

    prvRet = signal(SIGINT, myHandler);
    assert(prvRet != SIG_ERR);

    prvRet = signal(SIGTSTP, myHandler);
    assert(prvRet != SIG_ERR);

    // 강제로 SIGINT 발생
    // raise(SIGINT);
    
    while (1) {
        printf("I'm looping~\n");
        sleep(1);
    }
}