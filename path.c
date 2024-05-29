int is_absolute_path(const char *path) {
    return path[0] == '/'; // abs 경로인지 확인
}
