#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "gameInfo.h"   // server와 client가 공통으로 가지는 변수 정의

#define QUEUE_LEN           16
#define MAX_CLIENT_NUM      2
#define MESSAGE_QUEUE_KEY   1357
#define GAME_DATA           2468

// 메시지큐로 넘길 자료형 정의
typedef struct {
    long mtype;
    pthread_t tid;  // 해당 client를 관리하는 thread의 id
    int *client_fd;  // 해당 client와 연결된 socket의 file descriptor
    int info;   // 기타 정보를 담는 곳
    char msg[MAX_MSG_SIZE];    // 주고받을 메시지
} message_queue_node;
size_t msg_size = sizeof(message_queue_node) - sizeof(long);

/** client의 접속을 처리하는 thread function
 *  @param arg client와 연결된 socket의 file descriptor */
void *handleClientThread(void *arg);

/** 승자를 판별하는 thread function
 *  @param arg client와 접속을 관리하는 socket의 file descriptor */
void *judgeWinnerThread(void *arg);

/** 두 client의 입력을 통해 승자를 판단하는 배열
 *  1-index: 대상 client의 입력 정보 (Rock: 0, Scissor: 1, Paper: 2)
 *  2-index: 상대 client의 입력 정보 (Rock: 0, Scissor: 1, Paper: 2)
 *  배열값: 대상 client가 이기면 0, 비기면 1, 지면 2 */ 
const int judge_table[3][3] = {
    {1, 0, 2},
    {2, 1, 0},
    {0, 2, 1}};

// 정수 입력 값을 문자열로 변환
const char game_input_table[3][MAX_MSG_SIZE] = {"Rock", "Scissor", "Paper"};

// 정수 결과 값을 문자열로 변환
const char game_result_table[3][MAX_MSG_SIZE] = {"win", "draw", "lose"};

int visit_count = 0;     // 현재 접속자 수
pthread_mutex_t visit_mutex;    // 접속자 수를 관리하는 mutex
char buf[MAX_TEXT_SIZE];     // 메시지를 담는 버퍼

// 실행 시 인자로 port 번호를 입력받음(default: PROTOCOL_PORT)
int main(int argc, char *argv[]) {
    struct hostent *host_ptr;
    struct protoent *proto_ptr;
    struct sockaddr_in server_addr;
    pthread_t client_tid, judge_tid;

    // mutex 초기화
    if (pthread_mutex_init(&visit_mutex, NULL) != 0) {
        fprintf(stderr, "Mutex init fail.\n");
        exit(1);
    }

    // server socket address 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;   // IPv4 인터넷 프로토콜로 설정
    server_addr.sin_addr.s_addr = INADDR_ANY;   // local IP로 설정

    // 인자가 입력된 경우, 해당 값으로 port를 설정
    int port = (argc == 1 ? PROTOCOL_PORT : atoi(argv[1]));
    
    // port의 유효성을 판단
    // (well-known port가 아니고, unsigned short 범위 내 인지 확인)
    if (port < (1 << 10) || port >= (1 << 16)) {
        fprintf(stderr, "%s is bad port number.\n", argv[1]);
        exit(1);
    }

    // 유효한 port 번호를 서버의 port로 설정
    server_addr.sin_port = htons((unsigned short)port);

    // TCP 프로토콜을 가져와 proto_ptr에 저장
    if ((proto_ptr = getprotobyname("tcp")) == NULL) {
        fprintf(stderr, "Can't map 'tcp' to protocol number.\n");
        exit(1);
    }

    // 소켓 file descriptor의 주소를 담을 변수 동적할당
    int *socket_fd_addr = (int*)malloc(sizeof(int));

    // socket() -> 소켓 생성
    if (((*socket_fd_addr) = socket(PF_INET, SOCK_STREAM, proto_ptr->p_proto)) < 0) {
        fprintf(stderr, "Socket creation fail.\n");
        exit(1);
    }

    int socket_fd = *socket_fd_addr;

    // bind() -> 소켓을 바인딩 해 외부의 연결을 받아들임
    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Socket binding fail.\n");
        exit(1);
    }

    // listen() -> 이제부터 요청을 대기
    if (listen(socket_fd, QUEUE_LEN) < 0) {
        fprintf(stderr, "Socket listening fail.\n");
        exit(1);
    }

    printf("Server is running...\n");

    // 승자를 판정하는 judgeWinnerThread 실행
    pthread_create(&judge_tid, NULL, judgeWinnerThread, (void*)(socket_fd_addr));

    while (1) {
        printf("SERVER: waiting...\n");

        // server가 client의 요청을 수락
        int accepted_fd = accept(socket_fd, NULL, NULL);
        if (accepted_fd < 0) {
            fprintf(stderr, "Socket accept fail.\n");
            exit(1);
        }

        // serverThread를 통해서 처리
        int *client_fd = (int*)malloc(sizeof(int));
        *client_fd = accepted_fd;
        pthread_create(&client_tid, NULL, handleClientThread, (void*)(client_fd));
    }
}

void *handleClientThread(void *arg) {
    int thread_fd = *((int*)arg), reject_flag = 0, cur_visit;
    char prefix[] = "handleClientThread";

    // visit_count라는 전역 변수에 접근하므로 mutex로 critical section 확보
    pthread_mutex_lock(&visit_mutex);

    // 접속자가 2명을 초과한 경우 -> 해당 접속자를 내쫓아야 함
    if (++visit_count > MAX_CLIENT_NUM) {
        reject_flag = 1;
        --visit_count;
    }
    cur_visit = visit_count;

    pthread_mutex_unlock(&visit_mutex);

    if (reject_flag) {
        // 해당 client에게 reject 메시지 전송
        sprintf(buf, "%c connection rejected. (Two clients already connected.)\n", BAD_CONNECTION);
        send(thread_fd, buf, strlen(buf), 0);
        printf("%s: connection rejected. (Two clients already connected.)\n", prefix);

        close(thread_fd);   // 해당 소켓을 닫음
        free(arg);
        pthread_exit(NULL);
    }

    // 연결되었다는 메시지를 보냄
    pthread_t pid = pthread_self();
    sprintf(buf, "%c your PID in server thread is %lu.\n", GOOD_CONNECTION, pid);
    send(thread_fd, buf, strlen(buf), 0);
    printf("%s: %d client%s connected! (PID: %lu)\n\n", prefix, cur_visit, (cur_visit == 1 ? " is" : "s are"), pid);

    // judgeWinnerThread로 메시지를 보낼 messageQueue의 아이디를 설정
    int qid;
    if ((qid = msgget(MESSAGE_QUEUE_KEY, 0666 | IPC_CREAT)) == -1) {
        fprintf(stderr, "msgget error. (key: %d)\n", MESSAGE_QUEUE_KEY);
        close(thread_fd);
        free(arg);
        pthread_exit(NULL);
    }

    int client_input;   // client의 입력을 정수로 저장 (Rock: 0, Scissor: 1, Paper: 2)
    while (1) {
        // client의 입력을 받음
        int n = recv(thread_fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        
        buf[n] = '\0';
        printf("%s: client's input is '%s'. (PID: %lu)\n", prefix, (buf+2), pid);

        int break_flag = 0;

        client_input = 3;
        switch (buf[2]) {
        // 올바른 입력 -> 잘 받았다고 client로 보냄
        case 'R': case 'r': --client_input;
        case 'S': case 's': --client_input;
        case 'P': case 'p': --client_input;
            sprintf(buf, "%c you type '%s'!\n", GOOD_INPUT, game_input_table[client_input]);
            send(thread_fd, buf, strlen(buf), 0);
            break_flag = 1;
            break;

        // 종료 입력 -> 해당 thread를 종료
        case 'Q': case 'q':
            // client가 종료되었으므로 visit_count 감소시킴
            pthread_mutex_lock(&visit_mutex);
            --visit_count;
            pthread_mutex_unlock(&visit_mutex);

            sprintf(buf, "%c Quit client process.\n", QUIT_CLIENT);
            send(thread_fd, buf, strlen(buf), 0);
            printf("%s: disconnect client. (PID: %lu)\n", prefix, pid);

            close(thread_fd);   // 해당 소켓을 닫음
            free(arg);
            pthread_exit(NULL);
        
        // 올바르지 않은 입력 -> 다시 요청
        default:
            sprintf(buf, "%c Invalid input.\n\n", REQUEST_INPUT);
            send(thread_fd, buf, strlen(buf), 0);
            break;
        }

        if (break_flag) break;
    }

    // 받은 정보를 judgeWinnerThread로 전송
    message_queue_node msg = {GAME_DATA, pid, arg, client_input};
    sprintf(msg.msg, "%s", game_input_table[client_input]);
    if (msgsnd(qid, (void*)&msg, msg_size, 0) == -1) {
        fprintf(stderr, "message send error.\n");
        close(thread_fd);
        free(arg);
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

void *judgeWinnerThread(void *arg){
    int socket_fd = *((int*)arg);
    char prefix[] = "judgeWinnerThread";

    printf("judge winner thread is running...\n");

    // 메시지를 받을 messageQueue의 아이디를 설정
    int qid;
    if ((qid = msgget(MESSAGE_QUEUE_KEY, 0666 | IPC_CREAT)) == -1) {
        fprintf(stderr, "msgget error. (key: %d)\n", MESSAGE_QUEUE_KEY);
        pthread_exit(NULL);
    }

    // 첫번째 client로부터 메시지를 받음
    message_queue_node msg1;
    if (msgrcv(qid, &msg1, msg_size, GAME_DATA, 0) == -1) {
        fprintf(stderr, "msgrcv error.\n");
        pthread_exit(NULL);
    }
    printf("%s: message received from client. (PID: %lu)\n\n", prefix, msg1.tid);

    // 두번째 client로부터 메시지를 받음
    message_queue_node msg2;
    if (msgrcv(qid, &msg2, msg_size, GAME_DATA, 0) == -1) {
        fprintf(stderr, "msgrcv error.\n");
        pthread_exit(NULL);
    }
    printf("%s: message received from client. (PID: %lu)\n\n", prefix, msg2.tid);

    // 첫번째 client에게 판정 결과 메시지 보냄
    int first_result = judge_table[msg1.info][msg2.info], first_fd = *(msg1.client_fd);
    sprintf(buf, "%c you %s! (you: '%s', opponent: '%s')", NONE, game_result_table[first_result], msg1.msg, msg2.msg);
    send(first_fd, buf, strlen(buf), 0);
    printf("%s: client(PID: %lu) is %s.\n", prefix, msg1.tid, game_result_table[first_result]);
    close(first_fd);
    free(msg1.client_fd);

    // 두번째 client에게 판정 결과 메시지 보냄
    int secont_result = judge_table[msg2.info][msg1.info], second_fd = *(msg2.client_fd);
    sprintf(buf, "%c you %s! (you: '%s', opponent: '%s')", NONE, game_result_table[secont_result], msg2.msg, msg1.msg);
    send(second_fd, buf, strlen(buf), 0);
    printf("%s: client(PID: %lu) is %s.\n", prefix, msg2.tid, game_result_table[secont_result]);
    close(second_fd);
    free(msg2.client_fd);

    // 판정 끝 -> 게임 종료
    printf("%s: judging game result is done.\n\n", prefix);
    close(socket_fd);
    free(arg);
    exit(0);
}