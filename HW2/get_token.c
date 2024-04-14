#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_shell.h"

char** getToken(char *line)
{
    int idx = 0;

    // 토큰화의 구분자를 선언
    char delim[] = " \t\r\n";

    /*  MAX_ARGC개의 토큰을 담고, 마지막에 NULL문자를 삽입하여 끝을 표시하기 위해서
        (MAX_ARGC + 1) * sizeof(char*)만큼의 공간을 할당 */
    char **token_list = (char**)malloc((MAX_ARGC + 1) * sizeof(char*));

    // 동적할당 실패
    if (token_list == NULL) {
        printf("malloc error.\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, delim);
    while (token != NULL)
    {
        // idx번째 칸에 토큰화된 token을 넣을 수 있음
        // 이때, 포인터를 넘겨주기 때문에 그냥 대입하기
        token_list[idx++] = token;

        if (idx == MAX_ARGC)
        {
            printf("Too many args.\n");
            break;
        }

        // 이어서 토큰화 진행
        token = strtok(NULL, delim);
    }

    // 배열의 끝을 표시
    token_list[idx] = NULL;

    return token_list;
}