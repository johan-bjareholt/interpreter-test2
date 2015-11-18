
NAME=compiler

SRC=main.c

CFLAGS += --std=c99 -Wall -g

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(NAME)
