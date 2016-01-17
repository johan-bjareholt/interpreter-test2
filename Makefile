
NAME=interpreter

SRCF	= main.c parser.c datatypes.c eval.c input.c variables.c section.c
SRC	= $(patsubst %,src/%,$(SRCF))

CFLAGS += -std=gnu99 -Wall -g

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(NAME)
