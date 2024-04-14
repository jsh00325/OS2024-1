#define MAX_ARGC 20     // 명령어의 최대 단어 수
#define MAX_LINE 80     // 명령어의 최대 길이

/** 입력된 한 줄의 문장을 토큰화하는 함수
 *  @param line 토큰화하고 싶은 문자열의 포인터
 *  @return 토큰화 한 문장을 문자열들을 배열로 반환 -> 이중 포인터 */
char** getToken(char *line);

/** 토큰화된 명령어를 실행하는 함수
 *  @param args 토큰화된 문자열 배열 */
int execute(char **args);