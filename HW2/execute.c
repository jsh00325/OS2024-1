#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "my_shell.h"

int execute(char **args) 
{   
    // 수행하고자 하는 명령이 없는 경우
    if (args[0] == NULL)
    {
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) // 프로세스 fork 오류
    {
        printf("Fork error!\n");
        return 1;
    }
    else if (pid == 0) // 자식 프로세스
    {   
        int status = execvp(args[0], args);

        // execvp가 실행 실패했을 때, 메시지 출력
        printf("'%s': Execute error!(%d)\n", args[0], status);
        exit(EXIT_FAILURE);
    }
    else // 부모 프로세스
    {
        // 자식 프로세스가 종료될 때 까지 대기
        wait(NULL);
    }

    return 0;
}