
NAME=interpreter

SRCF	= main.c parser.c datatypes.c eval.c input.c variables.c
SRC	= $(patsubst %,src/%,$(SRCF))

CFLAGS += --std=c99 -Wall -g

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(NAME)
