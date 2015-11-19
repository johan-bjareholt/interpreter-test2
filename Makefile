
NAME=interpreter

SRC=main.c parser.c datatypes.c eval.c input.c variables.c

CFLAGS += --std=c99 -Wall -g

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(NAME)
