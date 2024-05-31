#include "tush.h"

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\n종료하려면 'exit'를 입력하세요\n");
    }
}

void setup_environment() {
    setenv("PATH", "/bin", 1); // 환경 변수 설정 함수 : PATH 변수 /bin 설정
}