# 컴파일러
CC = gcc

# 컴파일러 옵션
CFLAGS = -Wall -g

# 타겟 실행 파일
TARGET = tush

# 소스 파일
SRCS = exe_command.c parse_args.c path.c process_input.c redirect.c signal.c tush.c

# 오브젝트 파일
OBJS = $(SRCS:.c=.o)

# 디펜던시 파일
DEPS = $(SRCS:.c=.d)

# 기본 타겟
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 디펜던시 파일 생성 규칙
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# 디펜던시 포함
-include $(DEPS)

# 클린 규칙
clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)

