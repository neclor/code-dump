#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static inline long long_hash_by_ptr(const long *ptr) { return (size_t)(*(const long *)ptr); }

static inline bool long_equal_by_ptr(const long *ptr1, const long *ptr2) { return *(const long *)ptr1 == *(const long *)ptr2; }

static inline size_t size_t_hash_by_ptr(const size_t *ptr) { return (size_t)(*(const size_t *)ptr); }

static inline bool size_t_equal_by_ptr(const size_t *ptr1, const size_t *ptr2) { return *(const size_t*)ptr1 == *(const size_t*)ptr2; }

size_t file_hash(FILE *file);

size_t file_hash_by_path(const char *path);

size_t file_hash_by_path_ptr(const char **path_ptr);

size_t string_hash(const char *str);

size_t string_hash_by_ptr(const char **str_ptr);

bool string_equal_by_ptr(const char **str_ptr_a, const char **str_ptr_b);

#endif // HASH_FUNCTIONS_H
