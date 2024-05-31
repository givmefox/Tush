#include "tush.h"

void parse_command(char *cmdline, char **argv, int *background) {
    char *token;
    int argc = 0;
    char *rest = cmdline; //strsep는 원본 문자열을 보존하지 않음

    while ((token = strsep(&rest, " \t\n")) != NULL) {      //strsep()로 문자열 파싱
        if (*token == '\0') continue; // 빈 토큰 건너뛰기
        if (strcmp(token, "&") == 0) {
            *background = BACKGROUND; // & 만나면 bg 실행 설정
        } else {
            argv[argc++] = token; // 인자를 배열에 저장
        }
    }
    argv[argc] = NULL; // 마지막 인자에 널 포인터 설정
}
