
NAME=interpreter

SRCF= main.c input.c lexer.c token.c parser.h
SRC	= $(patsubst %,src/%,$(SRCF))

CFLAGS += -std=gnu99 -Wall -g

all:
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
