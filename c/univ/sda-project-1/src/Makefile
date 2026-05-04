.PHONY: all clean run

# -----
SRC_DIR	= .
INC = $(addprefix -I, $(shell find $(SRC_DIR) -type d))
# -----

CC = gcc
CFLAGS = -g -Wall -W -Wextra -Wmissing-prototypes --pedantic -std=c99 $(INC)
LDFLAGS = -lm

all: autocompletelinear autocompletesort_qs autocompletesort_ms autocompletepq_qs autocompletepq_ms bench_qs bench_ms bench_pq_qs bench_pq_ms

clean:
	rm -rf */*.o *.o autocompletelinear autocompletesort_* autocompletepq_* bench_*

run: autocompletelinear
	./autocompletelinear -i Data/baby-names.txt 20

autocompletelinear: main.o Terms/Terms.o  AutoComplete/AutoCompleteLinear.o
	$(CC) -o $@ $^ $(LDFLAGS) -g -lncurses

autocompletesort_qs: main.o Terms/Terms.o  Sort/QuickSort.o AutoComplete/AutoCompleteSort.o Search/BinarySearch.o
	$(CC) -o $@ $^ $(LDFLAGS) -g -lncurses

autocompletesort_ms: main.o Terms/Terms.o  Sort/MergeSort.o AutoComplete/AutoCompleteSort.o Search/BinarySearch.o
	$(CC) -o $@ $^ $(LDFLAGS) -g -lncurses

autocompletepq_qs: main.o Terms/Terms.o  Sort/QuickSort.o AutoComplete/AutoCompletePQ.o Search/BinarySearch.o PQ/PQ.o
	$(CC) -o $@ $^ $(LDFLAGS) -g -lncurses

autocompletepq_ms: main.o Terms/Terms.o  Sort/MergeSort.o AutoComplete/AutoCompletePQ.o Search/BinarySearch.o PQ/PQ.o
	$(CC) -o $@ $^ $(LDFLAGS) -g -lncurses

AutoComplete/AutoCompleteSort.o: AutoComplete/AutoCompleteSort.c AutoComplete/AutoComplete.h Terms/Terms.h Sort/Sort.h Search/BinarySearch.h
AutoComplete/AutoCompletePQ.o: AutoComplete/AutoCompletePQ.c AutoComplete/AutoComplete.h Terms/Terms.h Sort/Sort.h Search/BinarySearch.h
AutoComplete/AutoCompleteLinear.o: AutoComplete/AutoCompleteLinear.c AutoComplete/AutoComplete.h Terms/Terms.h
Terms/Terms.o: Terms/Terms.c Terms/Terms.h
main.o: main.c Terms/Terms.h
Sort/MergeSort.o: Sort/MergeSort.c Sort/Sort.h
Sort/QuickSort.o: Sort/QuickSort.c Sort/Sort.h
Search/BinarySearch.o: Search/BinarySearch.c Search/BinarySearch.h
PQ/PQ.o: PQ/PQ.c PQ/PQ.h

bench_qs: bench.o Terms/Terms.o  Sort/QuickSort.o AutoComplete/AutoCompleteSort.o Search/BinarySearch.o
	$(CC) -o $@ $^ $(LDFLAGS)

bench_ms: bench.o Terms/Terms.o  Sort/MergeSort.o AutoComplete/AutoCompleteSort.o Search/BinarySearch.o
	$(CC) -o $@ $^ $(LDFLAGS)

bench_pq_qs: bench.o Terms/Terms.o  Sort/QuickSort.o AutoComplete/AutoCompletePQ.o Search/BinarySearch.o PQ/PQ.o
	$(CC) -o $@ $^ $(LDFLAGS)

bench_pq_ms: bench.o Terms/Terms.o  Sort/MergeSort.o AutoComplete/AutoCompletePQ.o Search/BinarySearch.o PQ/PQ.o
	$(CC) -o $@ $^ $(LDFLAGS)

bench.o: bench.c Terms/Terms.h Sort/Sort.h AutoComplete/AutoComplete.h
	$(CC) $(CFLAGS) -c bench.c -o $@

bench: bench_qs bench_ms
	@echo "=== QUICK SORT ==="
	./bench_qs actors_25k.txt actors_250k.txt actors_2500k.txt
	@echo "=== MERGE SORT ==="
	./bench_ms actors_25k.txt actors_250k.txt actors_2500k.txt
