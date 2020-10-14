CC = gcc
CFLAGS += -Wall -std=c11 -Wextra -Werror
OBJS = tetrominoes.o main.o

.PHONY: all clean

all: main

main: $(OBJS)
	@echo "Compiling main..."
	@$(CC) $(CFLAGS) $(OBJS) -o playGE -lncurses


clean:
	@echo "Cleaning..."
	@rm -f $(OBJS) playGE
