#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64

void execute_command(char *cmd) {
    char *args[MAX_ARGS];
    int i = 0;

    // 토큰화하여 명령어와 인자를 분리합니다.
    args[i] = strtok(cmd, " \t\n");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " \t\n");
    }
    args[i] = NULL;  // 인자 리스트의 끝을 NULL로 설정합니다.

    // 자식 프로세스를 생성하여 명령어를 실행합니다.
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 자식 프로세스
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // 부모 프로세스
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
}

int main() {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        printf("> ");
        if (getline(&line, &len, stdin) == -1) {
            perror("getline");
            break;
        }

        // 종료 명령어 처리
        if (strcmp(line, "exit\n") == 0) {
            break;
        }

        execute_command(line);
    }

    free(line);
    return 0;
}

