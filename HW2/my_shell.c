#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "my_shell.h"

void settingPath();

int main(void)
{
    char line[MAX_LINE];
    char **args;

    int status = 0;

    printf("\nMy Shell Started...\n");

    // mybin에 있는 파일을 바로 실행하기 위해 PATH 추가
    settingPath();

    while (1) {
        // 명령어 입력
        printf("MyShell> ");
        fgets(line, MAX_LINE, stdin);

        if (strncmp(line, "quit", 4) == 0) {
            // 명령어의 앞 4자리가 "quit"인 경우 종료하기
            exit(0);
        }
        else {
            // 입력된 명령어를 토큰화하여 저장
            args = getToken(line);

            // 토큰화된 명령어를 실행
            status = execute(args);

            // 실행이 끝났기 때문에, args 할당 해제
            free(args);

            printf("\n");
        }
    }

    return 0;
}

void settingPath()
{   
    // 현재 시스템의 PATH 값에 현재 폴더의 하위폴더인 mybin 폴더를 추가
    char* path = getenv("PATH");
    strcat(path, ":./mybin/");

    // mybin이 포함된 PATH를 적용
    if (setenv("PATH", path, 1) != 0) {
        printf("Path Setting Error!\n");
        exit(1);
    }
}