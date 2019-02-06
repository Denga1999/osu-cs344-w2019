CC = gcc
CFLAGS = -ansi -g -Wall -Wextra

ALL = luuph.buildrooms luuph.adventure

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(ALL)

luuph.buildrooms: luuph.buildrooms.o
	$(CC) $(CFLAGS) $? -o $@

luuph.adventure: luuph.adventure.o
	$(CC) $(CFLAGS) -lpthread $? -o $@

.PHONY: clean

clean:
	rm -f $(ALL) *.o
