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
void parse_command(char *cmdline, char **argv, int *background);
void redirect_io(char *input_file, char *output_file, char *error_file);
void execute_command(char **argv, int background, char *input_file, char *output_file, char *error_file);
void process_input(char *cmdline, char **argv, int *background, char **input_file, char **output_file, char **error_file);