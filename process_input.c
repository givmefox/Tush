#include "tush.h"

void process_input(char *cmdline, char **argv, int *background, char **input_file, char **output_file, char **error_file) {
    // 리다이렉션 처리
    char *input_redirect = strchr(cmdline, '<');
    char *output_redirect = strchr(cmdline, '>');
    char *error_redirect = strstr(cmdline, "2>");

    if (input_redirect) {
        *input_redirect = '\0';
        *input_file = strtok(input_redirect + 1, " \t\n");
    }

    if (output_redirect) {
        *output_redirect = '\0';
        *output_file = strtok(output_redirect + 1, " \t\n");
    }

    if (error_redirect) {
        *error_redirect = '\0';
        *error_file = strtok(error_redirect + 2, " \t\n");
    }

    parse_command(cmdline, argv, background); // 명령어 파싱
}
