#include "tush.h"

void execute_command(char **argv, int background, char *input_file, char *output_file, char *error_file) {
    if (access(argv[0], F_OK) != 0 && !is_absolute_path(argv[0])) {
        char *path_env = getenv("PATH");
        char *path = strtok(path_env, ":");
        while (path != NULL) {
            char full_path[MAXLINE];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, argv[0]);
            if (access(full_path, F_OK) == 0) {
                argv[0] = strdup(full_path);
                break;
            }
            path = strtok(NULL, ":");
        }
    }

    pid_t pid = fork();         //자식 프로세스 생성
    if (pid == 0) {             //자식프로세스이면
        redirect_io(input_file, output_file, error_file);
        execvp(argv[0], argv); // 명령어 실행
        perror(argv[0]); // 실패 시 에러 메시지 출력
        exit(1);
    } else if (pid > 0) {           //부모프로세스이면
        if (background == BACKGROUND) {
            printf("[Process id %d]\n", pid); // bg 실행 시 자식 프로세스 ID 출력
        } else {
            waitpid(pid, NULL, 0); // fg 실행 시 자식 프로세스 종료 대기
        }
    } else {
        perror("fork"); // fork 실패 시 에러 출력
    }
}
