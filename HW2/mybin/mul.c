#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf(" mul> ");

    // argument가 입력되지 않은 경우
    if (argc == 1)
    {
        printf("there is no argument.\n");
        return 0;
    }

    // 결과를 저장할 변수
    int ans = 1;

    // add와 마찬가지로, idx가 1번부터 구하고자 하는 정보가 들어옴
    for (int i = 1; i < argc; ++i)
    {
        // 마찬가지로 argv[i]는 문자열 형태이므로 atoi함수 사용
        int cur_value = atoi(argv[i]);
        ans *= cur_value;

        if (cur_value > 0) printf("%d ", cur_value);
        else printf("(%d) ", cur_value);

        if (i < argc - 1) printf("x ");
        else printf("= ");
    }

    // 구한 결과 출력
    printf("%d\n", ans);
    
    return 0;
}