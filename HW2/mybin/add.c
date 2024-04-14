#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf(" add> ");

    // argument가 입력되지 않은 경우
    if (argc == 1)
    {
        printf("there is no argument.\n");
        return 0;
    }
    
    // 결과를 저장할 변수
    int ans = 0;

    // idx 1번부터 구하고자 하는 정보가 들어옴
    for (int i = 1; i < argc; ++i)
    {
        // argv[i]는 문자열 형태이므로 atoi함수 사용
        int cur_value = atoi(argv[i]);
        ans += cur_value;

        if (cur_value > 0) printf("%d ", cur_value);
        else printf("(%d) ", cur_value);

        if (i < argc - 1) printf("+ ");
        else printf("= ");
    }

    // 구한 결과 출력
    printf("%d\n", ans);
    
    return 0;
}