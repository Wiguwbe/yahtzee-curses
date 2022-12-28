TARGET=yahtzee
OBJS=colors.o draw.o sm.o main.o
LIBS=-lm -lncurses

CC=gcc
CFLAGS=

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c $<

clean:
	rm *.o
