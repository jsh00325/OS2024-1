// 기본으로 연결되는 포트 번호
#define PROTOCOL_PORT   7361

// socket으로 주고받는 버퍼의 최대 크기
#define MAX_TEXT_SIZE   1024

// 메시지 큐로 주고받는 메시지의 크기
#define MAX_MSG_SIZE    32

// socket으로 주고 받는 메시지에 추가적인 정보를 주는 변수
#define NONE            ' '
#define GOOD_CONNECTION '!'
#define BAD_CONNECTION  '@'
#define REQUEST_INPUT   '#'
#define GOOD_INPUT      '$'
#define QUIT_CLIENT     '%'