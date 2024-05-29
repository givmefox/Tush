void parse_command(char *cmdline, char **argv, int *background) {
    char *token;
    int argc = 0;

    token = strtok(cmdline, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = BACKGROUND; // & 만나면 bg 실행 설정
        } else {
            argv[argc++] = token; // 인자를 배열에 저장
        }
        token = strtok(NULL, " \t\n"); // 다음 토큰 이동
    }
    argv[argc] = NULL; // 마지막 인자에 널 포인터 설정
}
