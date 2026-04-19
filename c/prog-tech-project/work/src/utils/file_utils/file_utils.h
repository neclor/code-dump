#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Preserves file position */
long file_size(FILE *file);

/* Compares content. Preserves file positions */
bool file_equal(FILE *a, FILE *b);

bool file_equal_by_path(const char *path_a, const char *path_b);

bool file_equal_by_path_ptr(const char **path_ptr_a, const char **path_ptr_b);

#endif // FILE_UTILS_H
