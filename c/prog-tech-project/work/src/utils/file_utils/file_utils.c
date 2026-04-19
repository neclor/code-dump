#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "file_utils.h"

#define FILE_UTILS_BUFFER_SIZE (4096)

long file_size(FILE *file) {
	if (!file) return -1;

	long position = ftell(file);
	if (fseek(file, 0, SEEK_END) != 0) return -1;
	long size = ftell(file);
	if (position != -1) fseek(file, position, SEEK_SET);

	return size;
}

bool file_equal(FILE *a, FILE *b) {
	if (!a || !b) return false;

	if (file_size(a) != file_size(b)) return false;

	uint8_t buffer_a[FILE_UTILS_BUFFER_SIZE];
	uint8_t buffer_b[FILE_UTILS_BUFFER_SIZE];
	size_t read_a;
	size_t read_b;

	long position_a = ftell(a);
	long position_b = ftell(b);
	rewind(a);
	rewind(b);

	do {
		read_a = fread(buffer_a, sizeof(uint8_t), FILE_UTILS_BUFFER_SIZE, a);
		read_b = fread(buffer_b, sizeof(uint8_t), FILE_UTILS_BUFFER_SIZE, b);
		if (read_a != read_b || memcmp(buffer_a, buffer_b, read_a) != 0) {
			if (position_a != -1) fseek(a, position_a, SEEK_SET);
			if (position_b != -1) fseek(b, position_b, SEEK_SET);
			return false;
		}
	} while (read_a == FILE_UTILS_BUFFER_SIZE);

	if (position_a != -1) fseek(a, position_a, SEEK_SET);
	if (position_b != -1) fseek(b, position_b, SEEK_SET);

	return true;
}

bool file_equal_by_path(const char *path_a, const char *path_b) {
	if (!path_a || !path_b) return false;

	FILE *file_a = fopen(path_a, "rb");
	FILE *file_b = fopen(path_b, "rb");
	if (!file_a || !file_b) {
		if (file_a) fclose(file_a);
		if (file_b) fclose(file_b);
		return false;
	}

	bool equal = file_equal(file_a, file_b);

	fclose(file_a);
	fclose(file_b);

	return equal;
}

bool file_equal_by_path_ptr(const char **path_ptr_a, const char **path_ptr_b) {
	return path_ptr_a && path_ptr_b && file_equal_by_path(*path_ptr_a, *path_ptr_b);
}
