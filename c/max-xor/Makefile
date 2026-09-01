CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Isrc/trie
SRC = src/trie/trie.c src/main.c
BIN = max_xor

$(BIN): $(SRC) src/trie/trie.h
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: run clean
