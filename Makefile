
NAME=interpreter

SRCF= main.c input.c lexer.c token.c ast_builder.c tokentable.c
SRC	= $(patsubst %,src/%,$(SRCF))

CFLAGS += -std=c11 -Wall -g

all:
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
