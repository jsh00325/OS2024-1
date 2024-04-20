#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Origin form of Signal Handler
//  -> return previous handler function pointer (if success)
//  -> or return SIG_ERR (if error)
void (*signal (int sig, void (*func)(int))) (int);

// signal handlers
void ouch(int sig) {
    printf("\nOuch! I got SIGINT(signal %d)!\n", sig);

    // set handler to default
    (void) signal(SIGINT, SIG_DFL);
}
void dying(int sig) {
    printf("\nI got SIGKILL(%d).\n", sig);
    (void) signal(SIGKILL, SIG_DFL);
}
void stopping(int sig) {
    printf("\nOh... I got SIGTSTP(%d).\n", sig);
    (void) signal(SIGTSTP, SIG_DFL);
}
void quitHandler(int sig) {
    printf("\nI got SIGQUIT(%d).\n", sig);
    (void) signal(SIGQUIT, SIG_DFL);
}
void abortHandler(int sig) {
    printf("\nI got SIGABRT(%d).\n", sig);
    (void) signal(SIGABRT, SIG_DFL);
}

int main() {
    // setting signal handler
    (void) signal(SIGINT, ouch);
    (void) signal(SIGKILL, dying);
    (void) signal(SIGTSTP, stopping);
    (void) signal(SIGQUIT, quitHandler);
    (void) signal(SIGABRT, abortHandler);

    while (1) {
        printf("I'm looping~\n");
        sleep(1);
    }
}