#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int ctrl_c_count;
#define CTRL_C_THRESHOLD 5

/* Ctrl+C signal handler */
void catch_int(int sig) {
    sigset_t mask_set, old_set;

    (void) signal(SIGINT, catch_int);
    
    sigfillset(&mask_set);
    sigprocmask(SIG_SETMASK, &mask_set, &old_set);
    
    if (++ctrl_c_count >= CTRL_C_THRESHOLD) {
        char answer[30];

        printf("\nRealy Exit? [y/N]: ");
        fflush(stdout);
        
        fgets(answer, 30, stdin);
        if (answer[0] == 'y' || answer[0] == 'Y') {
            printf("\nExiting...\n");
            fflush(stdout);
            exit(0);
        }
        else {
            printf("\nContinuing\n");
            fflush(stdout);

            /* reset Ctrl+C counter */
            ctrl_c_count = 0;
        }
    }
    sigprocmask(SIG_SETMASK, &old_set, NULL);
}

/* Ctrl+Z signal handler */
void catch_suspend(int sig) {
    sigset_t mask_set, old_set;

    signal(SIGTSTP, catch_suspend);

    sigfillset(&mask_set);
    sigprocmask(SIG_SETMASK, &mask_set, &old_set);

    printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
    fflush(stdout); 

    sigprocmask(SIG_SETMASK, &old_set, NULL);
}

int main() {
    (void) signal(SIGINT, catch_int);
    (void) signal(SIGTSTP, catch_suspend);

    while (1) {
        pause();
    }
    return 0;
}