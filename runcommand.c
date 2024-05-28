#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define MAXLINE 1024 // 입력 받을 최대 라인 길이
#define MAXARGS 128 // Maximum 명령어 인자 수
#define BACKGROUND 1 // bg 실행 상수 정의
#define FOREGROUND 0 // fg 실행 상수 정의

void handle_signal(int sig);
void setup_environment();
int is_absolute_path(const char *path);
int runcommand(char **cline, int where);
void parse_command(char *cmdline, char **argv, int *background);

int main() {
    char cmdline[MAXLINE]; // 명령어 입력 저장할 배열
    char *argv[MAXARGS]; // 명령어 인자 저장할 배열
    int background; // bg 실행 여부 저장할 변수
    char *input_file = NULL, *output_file = NULL, *error_file = NULL;

    signal(SIGINT, handle_signal);
    // 강제종료 핸들러 설정
    setup_environment();
    // 환경 변수 설정
    while (1) {
        background = FOREGROUND; // 기본 실행 = fg 설정
        input_file = output_file = error_file = NULL;
        // 리다이렉션 파일 초기화

        printf("tush> "); // 프롬프트 출력
        if (fgets(cmdline, MAXLINE, stdin) == NULL) {
            break; // 널 체크 (EOF)
        }

        if (cmdline[strlen(cmdline) - 1] == '\n') {
            // 마지막 자리 개행 문자 제거 & 널 값 삽입
            cmdline[strlen(cmdline) - 1] = '\0';
        }

        if (strcmp(cmdline, "exit") == 0) {
            break; // exit 입력 시 루프 종료
        }

        // 리다이렉션 처리
        char *input_redirect = strchr(cmdline, '<');
        // 입력 리다이렉션 검색
        char *output_redirect = strchr(cmdline, '>');
        // 출력 리다이렉션 검색
        char *error_redirect = strstr(cmdline, "2>");
        // 에러 리다이렉션 검색

        if (input_redirect) {
            *input_redirect = '\0';
            // 리다이렉션 기호 널 문자 대체
            input_file = strtok(input_redirect + 1, " \t\n");
            // 입력 파일 이름 추출
        }

        if (output_redirect) {
            *output_redirect = '\0';
            output_file = strtok(output_redirect + 1, " \t\n");
        }

        if (error_redirect) {
            *error_redirect = '\0';
            error_file = strtok(error_redirect + 2, " \t\n");
        }

        parse_command(cmdline, argv, &background);
        // 명령어 파싱
        if (argv[0] == NULL) {
            continue; // 입력된 명령어 없을 시 계속 반복
        }

        if (access(argv[0], F_OK) != 0 && !is_absolute_path(argv[0])) {
            // 경로 확인 및 설정
            char *path_env = getenv("PATH"); 
            // PATH 환경 변수 가져오기
            char *path = strtok(path_env, ":");
            // :PATH 환경변수 :로 분할
            while (path != NULL) {
                char full_path[MAXLINE];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, argv[0]);
                if (access(full_path, F_OK) == 0) {
                    argv[0] = full_path; 
                    // 경로 존재 시 명령어 경로 설정
                    break;
                }
                path = strtok(NULL, ":"); // 다음 경로로 이동
            }
        }

        pid_t pid = fork();
        // 자식 프로세스 생성
        if (pid == 0) {
            // 자식 프로세스
            redirect_io(input_file, output_file, error_file);
            // 리다이렉션 설정
            execvp(argv[0], argv); // 명령어 실행
            perror(argv[0]); // 실패 시 에러 메세지 출력
            exit(1);
        } else if (pid > 0) {
            // 부모 프로세스
            if (background == BACKGROUND) {
                // bg 실행 시 자식 프로세스 ID 출력
                printf("[Process id %d]\n", pid);
                // fg 실행 시 자식 프로세스 종료 대기 
            } else {
                waitpid(pid, NULL, 0);
            }
        } else {
            perror("fork"); // fork 실패 시 에러 출력
        }
    }

    return 0;
}


void handle_signal(int sig) {
    // SIGIN (ctrl + c) 강제종료 무시     
    if (sig == SIGINT) {
        printf("\nType 'exit' to quit the shell.\n");
    }
}

void setup_environment() {
    // 환경 변수 설정 함수 : PATH 변수 /bin 설정
    setenv("PATH", "/bin", 1);
}

int is_absolute_path(const char *path) {
    // abs 경로인지 확인
    return path[0] == '/';
}

int runcommand(char **cline, int where) {
    // 명령어 실행
    pid_t pid;// 프로세스 id
    int status; // 자식 프로세스 종료 상태

    switch(pid = fork()) {
        case -1:
            // fork 실패 시
            perror("tush"); // 에러 메세지 출력
            return -1; // 1 | -1 = 시스템에 에러 알림(출력)
        case 0:
            // 자식 프로세스
            execvp(*cline, cline); // 명령어 실행
            perror(*cline); // 실헹 실패 시 에러 메세지 출력
            exit(1); // 비정상 종료
    }

    if (where == BACKGROUND) {
        // 백그라운드 실행인 경우
        printf("[프로세스 id %d]\n", pid);
        // 자식 프로세스 ID 출력
        return 0; 
    }

    if (waitpid(pid, &status, 0) == -1) {
        // 포어그라운드 실행인 경우엔 자식 프로세스 종료 까지 대기
        return -1; // 실패시 -1 반환
    } else {
        return status; // 자식 프로세스 종료 상태 반환
    }
}

void parse_command(char *cmdline, char **argv, int *background) {
    // 명령어 파싱 = 명령 & 인자 분리 및 백그라운드 실행 여부 결정
    char *token; // 각 토큰 저장 
    int argc = 0; // 인자 개수 counting

    token = strtok(cmdline, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            // & 만나면 bg 실행 설정
            *background = BACKGROUND;
        } else {
            argv[argc++] = token; // 인자를 배열에 저장
        }
        token = strtok(NULL, " \t\n"); // 다음 토큰 이동
    }
    argv[argc] = NULL; // 마지막 인자에 널 포인터 설정
}

void redirect_io(char *input_file, char *output_file, char *error_file) {
    // 입력, 출력, 에러 리다이렉션 설정 함수
    if (input_file) {
        // 입력 리다이렉션 설정
        int fd = open(input_file, O_RDONLY);
        // 입력 파일 열기
        if (fd == -1) {
            perror("open input_file");
            // 파일 열기 실패 시 에러 메세지 출력
            exit(1); // 비정상 종료
        }
        dup2(fd, STDIN_FILENO); // STDIN -> 파일 변경
        close(fd); // 파일 디스럽터 닫기
    }

    if (output_file) {
        // 출력 리다이렉션 설정
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open output_file");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (error_file) {
        int fd = open(error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open error_file");
            exit(1);
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
    }
}
