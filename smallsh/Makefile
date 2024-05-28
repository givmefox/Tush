CC 	= gcc
CFLAGS 	= 
EXEC	= smallsh
INCLS	= 
LIBS	=

OBJS = smallsh.o userin.o inarg.o procline.o runcommand.o

$(EXEC): $(OBJS)
	$(CC)  $(CFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

$(OBJS):
	$(CC)  $(CFLAGS)  $(INCLS)  -c  $*.c

clean:
	@echo "cleaning ..."
	@/bin/rm *.o $(EXEC)

