CC = gcc
# CFLAGS = -ansi -g -Wall -Wextra

ALL = luuph.buildrooms luuph.adventure

all: $(ALL)

luuph.buildrooms: luuph.buildrooms.c
	$(CC) $? -o $@

luuph.adventure: luuph.adventure.c
	$(CC) -lpthread $? -o $@

.PHONY: clean

clean:
	rm -f $(ALL)
