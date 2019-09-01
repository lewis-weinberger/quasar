.DEFAULT_GOAL := default

EXEC := quasar
OBJ  := main.o emulator.o parse.o console.o
INC  := config.h proto.h globals.h

CC     := gcc
CFLAGS := -Wall
LIBS   := -lncurses

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $<

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(EXEC)

.PHONY: default
default: $(EXEC)

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXEC)
