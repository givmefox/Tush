#include "tush.h"

int main() {
    char *cmdline = NULL; // 명령어 입력 저장할 배열
    size_t len = 0;     //입력 버퍼 길이
    char *argv[MAXARGS]; // 명령어 인자 저장할 배열
    int background; // bg 실행 여부 저장할 변수
    char *input_file = NULL, *output_file = NULL, *error_file = NULL;

    signal(SIGINT, handle_signal);
    setup_environment(); // 환경 변수 설정

    while (1) {
        background = FOREGROUND; // 기본 실행 foreground
        input_file = output_file = error_file = NULL; // 리다이렉션 파일 초기화

        printf("prompt> "); // 프롬프트 출력
        if (getline(&cmdline, &len, stdin) == -1) { // getline()으로 명령어 읽기
            break; // 널 체크 (EOF)
        }
        if (cmdline[strlen(cmdline) - 1] == '\n') {
            cmdline[strlen(cmdline) - 1] = '\0'; // 마지막 자리 개행 문자 제거 & 널 값 삽입
        }
        if (strcmp(cmdline, "exit") == 0) {
            exit(0); // exit 입력 시 종료
        }

            process_input(cmdline, argv, &background, &input_file, &output_file, &error_file); // 입력 처리 및 명령어 파싱

        if (argv[0] == NULL) {
            continue; // 입력된 명령어 없을 시 계속 반복
        }

        execute_command(argv, background, input_file, output_file, error_file); // 명령어 실행
    }

    free(cmdline);
    return 0;
}
