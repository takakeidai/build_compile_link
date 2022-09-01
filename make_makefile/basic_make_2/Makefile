PROGRAM = output
OBJS =  main.o vector.o angle.o
SRCS = $(OBJS:%.o=%.c)
CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
all:$(PROGRAM)
clean:
	rm -rf $(PROGRAM) *.o
$(PROGRAM):$(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
