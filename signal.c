#include "tush.h"

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nType 'exit' to quit the shell.\n");
    }
}

void setup_environment() {
    setenv("PATH", "/bin", 1); // 환경 변수 설정 함수 : PATH 변수 /bin 설정
}
