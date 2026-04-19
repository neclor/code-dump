#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filededup_test.h"

#define SEED 42

static void write_file(const char *path, size_t size, unsigned int file_seed) {
	FILE *f = fopen(path, "rb");
	if (f) { fclose(f); return; }

	f = fopen(path, "wb");
	if (!f) { printf("ERROR: %s\n", path); return; }

	srand(file_seed);
	char buf[4096];
	size_t remaining = size;
	while (remaining > 0) {
		size_t chunk = remaining < sizeof(buf) ? remaining : sizeof(buf);
		for (size_t i = 0; i < chunk; i++) buf[i] = (char)rand();
		fwrite(buf, 1, chunk, f);
		remaining -= chunk;
	}

	fclose(f);
}

void generate_different_sizes(int n, size_t base_size) {
	for (int i = 0; i < n; i++) {
		char path[256];
		size_t file_size = base_size + (size_t)(i * 100);
		snprintf(path, sizeof(path), "%sdiff_%zu_%d.bin", DIR, file_size, i);
		write_file(path, file_size, SEED + (unsigned)i);
	}
}

void generate_unique_same_size(int n, size_t size) {
	for (int i = 0; i < n; i++) {
		char path[256];
		snprintf(path, sizeof(path), "%suniq_%zu_%d.bin", DIR, size, i);
		write_file(path, size, SEED + (unsigned)i);
	}
}

void generate_identical(int n, size_t size) {
	for (int i = 0; i < n; i++) {
		char path[256];
		snprintf(path, sizeof(path), "%sdup_%zu_%d.bin", DIR, size, i);
		write_file(path, size, SEED);
	}
}
