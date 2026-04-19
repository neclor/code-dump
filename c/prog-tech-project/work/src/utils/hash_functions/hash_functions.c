#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hash_functions.h"

#define HASH_FUNCTIONS_BUFFER_SIZE (4096)

#define HASH_FUNCTIONS_SEED (42)
#define HASH_FUNCTIONS_ROWS (32)
#define HASH_FUNCTIONS_COLUMNS (256)

static size_t hash_table[HASH_FUNCTIONS_ROWS][HASH_FUNCTIONS_COLUMNS];
static bool hash_table_ready = false;

/* ======= Prototypes of static functions ======= */

static inline uint64_t rand64(void);
static void initialize_hash_table(void);

/* ======= Public functions ======= */

size_t file_hash(FILE *file) {
	if (!file) return 0;

	initialize_hash_table();

	uint8_t buffer[HASH_FUNCTIONS_BUFFER_SIZE];
	size_t hash = 0;

	rewind(file);

	size_t row = 0;
	size_t read;
	while ((read = fread(buffer, sizeof(uint8_t), HASH_FUNCTIONS_BUFFER_SIZE, file)) > 0) {
		for (size_t i = 0; i < read; i++) {
			hash ^= hash_table[row][buffer[i]];
			row = (row + 1) % HASH_FUNCTIONS_ROWS;
		}
	}
	return hash;
}

size_t file_hash_by_path(const char *path) {
	if (!path) return 0;

	FILE *file = fopen(path, "rb");
	if (!file) return 0;

	size_t hash = file_hash(file);
	fclose(file);
	return hash;
}

size_t file_hash_by_path_ptr(const char **path_ptr) {
	return path_ptr ? file_hash_by_path(*path_ptr) : 0;
}

size_t string_hash(const char *str) {
	if (!str) return 0;

	initialize_hash_table();

	size_t hash = 0;

	size_t row = 0;
	while (*str) {
		hash ^= hash_table[row][(uint8_t)(*str)];
		row = (row + 1) % HASH_FUNCTIONS_ROWS;
		str++;
	}
	return hash;
}

size_t string_hash_by_ptr(const char **str_ptr) {
	return str_ptr ? string_hash(*str_ptr) : 0;
}

bool string_equal_by_ptr(const char **str_ptr_a, const char **str_ptr_b) {
	return str_ptr_a && str_ptr_b && strcmp(*str_ptr_a, *str_ptr_b) == 0;
}

/* ======= Static functions ======= */

static inline uint64_t rand64(void) { return ((uint64_t)rand() << 32) | (uint64_t)rand(); }

static void initialize_hash_table(void) {
	if (hash_table_ready) return;

	srand(HASH_FUNCTIONS_SEED);

	for (size_t row = 0; row < HASH_FUNCTIONS_ROWS; row++) {
		for (size_t column = 0; column < HASH_FUNCTIONS_COLUMNS; column++) {
			hash_table[row][column] = (size_t)rand64();
		}
	}
	hash_table_ready = true;
}
