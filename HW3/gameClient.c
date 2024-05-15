#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "gameInfo.h"   // server와 client가 공통으로 가지는 변수 정의

char default_host[] = "localhost";
char buf[MAX_TEXT_SIZE];    // 메시지를 담는 버퍼
char ipt[MAX_TEXT_SIZE-2];    // 기타 입력을 담는 버퍼

int main(int argc, char *argv[]) {
    struct hostent *host_ptr;
    struct protoent *proto_ptr;
    struct sockaddr_in server_addr;

    int socket_fd, n;
    char received_info;

    // server socket address 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    // 인자로 host가 입력된 경우 이를 처리
    char *host = (argc > 1 ? argv[1] : default_host);
    if ((host_ptr = gethostbyname(host)) == NULL) {
        fprintf(stderr, "%s is invalid host.\n", argv[1]);
        exit(1);
    }

    // 인자로 port가 입력된 경우 이를 처리
    int port = (argc > 2 ? atoi(argv[2]) : PROTOCOL_PORT);
    if (port < (1 << 10) || port >= (1 << 16)) {
        fprintf(stderr, "%s is bad port number.\n", argv[2]);
        exit(1);
    }
    server_addr.sin_port = htons((unsigned short)port);

    // ip 주소를 server_addr로 복사
    memcpy(&server_addr.sin_addr, host_ptr->h_addr_list[0], host_ptr->h_length);

    // TCP 프로토콜을 가져와 proto_ptr에 저장
    if ((proto_ptr = getprotobyname("tcp")) == NULL) {
        fprintf(stderr, "Can't map 'tcp' to protocol number.\n");
        exit(1);
    }

    // socket() -> 소켓 생성
    if ((socket_fd = socket(PF_INET, SOCK_STREAM, proto_ptr->p_proto)) < 0) {
        fprintf(stderr, "Socket creation fail.\n");
        exit(1);
    }

    // connect() -> client의 소켓을 server의 소켓과 연결
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Socket connection fail.\n");
        exit(1);
    }

    // 서버와의 연결이 되었는지 메시지로 확인
    n = recv(socket_fd, buf, sizeof(buf), 0);
    if (n <= 0) exit(0);

    buf[n] = '\0';
    received_info = buf[0];
    printf("CLIENT: %s", (buf+2));

    if (received_info == BAD_CONNECTION) {
        printf("Exit your program.\n");
        close(socket_fd);
        exit(0);
    }

    while (1) {
        // 입력을 받아 서버로 넘김
        printf("CLIENT: type your input ([R]ock / [S]cissor / [P]aper or [Q]uit): ");
        scanf("%s", ipt); getchar();

        sprintf(buf, "%c %s", NONE, ipt);
        send(socket_fd, buf, strlen(buf), 0);

        // 서버로부터 입력이 올바른지 확인
        n = recv(socket_fd, buf, sizeof(buf), 0);
        if (n <= 0) break;

        buf[n] = '\0';
        received_info = buf[0];
        printf("CLIENT: %s", (buf+2));

        if (received_info == GOOD_INPUT) break;
        else if (received_info == QUIT_CLIENT) {
            close(socket_fd);
            exit(0);
        }
    }

    // 판정 결과를 server로 부터 받기
    n = recv(socket_fd, buf, sizeof(buf), 0);
    buf[n] = '\0';
    printf("CLIENT: %s\n", (buf+2));

    // socket 닫고 종료
    close(socket_fd);
    return 0;
}