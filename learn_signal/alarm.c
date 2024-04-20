#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void catch_alarm(int sig) {
    printf("\nOperation timed out. Exiting...\n\n");
    exit(0);
}

int main() {
    (void) signal(SIGALRM, catch_alarm);

    printf("User name: "); fflush(stdout);
    alarm(5);  // 10초 뒤 SIGALRM 발생

    char user[40];
    scanf("%s", user); getchar();
    alarm(0);   // SIGALRM 초기화
    
    adf();

    scanf("%s", user);
    return 0;
}