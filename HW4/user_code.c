#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 50

// 사용자의 입력을 담을 버퍼
char input_buffer[BUFFER_SIZE];

// device로부터 받은 결과를 담을 버퍼
char output_buffer[BUFFER_SIZE];

int main()
{
    // 내가 만든 디바이스 드라이버 열기
    int fd = open("/dev/hw4device", O_RDWR);
    if (fd < 0) {   // 실패한 경우
        printf("Fail to open my device...\n");
        return -1;
    }

    // kernel로 보낼 문자열을 입력 받음
    printf("\nInput String:\t");
    fgets(input_buffer, BUFFER_SIZE, stdin);

    // kernel에 문자열 쓰기
    write(fd, input_buffer, strlen(input_buffer));

    // kernel로부터 문자열 받기
    read(fd, output_buffer, BUFFER_SIZE);

    // 받은 문자열 출력
    printf("Output String:\t%s\n", output_buffer);

    close(fd);
    return 0;
}