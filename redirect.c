void redirect_io(char *input_file, char *output_file, char *error_file) {
    if (input_file) {
        int fd = open(input_file, O_RDONLY);
        if (fd == -1) {
            perror("open input_file");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (output_file) {
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
