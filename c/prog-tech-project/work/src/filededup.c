#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.h"

#include "file_utils.h"
#include "hash_functions.h"

#include "file_paths.h"
#include "filededup.h"

typedef Hashmap Filededup_t;

/* ======= Public functions ======= */

FILEDEDUP FDInit(void) {
	return (FILEDEDUP)hashmap_new(sizeof(long), sizeof(FilePaths *), (GetHashFunction)long_hash_by_ptr, (KeysEqualFunction)long_equal_by_ptr);
}

void FDFree(FILEDEDUP fd) {
	Hashmap *size_table = (Filededup_t *)fd;
	if (!size_table) return;

	HashmapIter *iter = hashmap_iter_new(size_table);
	while(hashmap_iter_valid(iter)) {
		FilePaths **file_paths;
		hashmap_iter_next(iter, NULL, (void **)&file_paths);
		file_paths_free(*file_paths);
	} hashmap_iter_free(iter);

	hashmap_free(size_table);
}

int FDCheck(FILEDEDUP fd, const char *filepath) {
	Hashmap *size_table = (Filededup_t *)fd;
	if (!size_table || !filepath) return FILEDEDUP_ERROR;

	FILE *file = fopen(filepath, "rb");
	if (!file) return FILEDEDUP_ERROR;

	long size = file_size(file);
	fclose(file);
	if (size == -1) return FILEDEDUP_ERROR;

	size_t filepath_len = strlen(filepath) + 1;
    char *path_copy = malloc(filepath_len);
    if (!path_copy) return FILEDEDUP_ERROR;
    memcpy(path_copy, filepath, filepath_len);

	FilePaths *file_paths = NULL;
	FilePaths **file_paths_ptr = hashmap_at_or_insert(size_table, &size, &file_paths);
	if (!file_paths_ptr) {
		free(path_copy);
		return FILEDEDUP_ERROR;
	}

	file_paths = *file_paths_ptr;
	if (!file_paths) {
		file_paths = file_paths_new(path_copy);
		if (!file_paths) {
			free(path_copy);
			return FILEDEDUP_ERROR;
		}

		*file_paths_ptr = file_paths;
		return FILEDEDUP_OK;
	}

	if (!file_paths_add(file_paths, path_copy)) {
		free(path_copy);
		return FILEDEDUP_ERROR;
	}

	return FILEDEDUP_OK;
}

char **FDDump(FILEDEDUP fd, int *length) {
	Hashmap *size_table = (Filededup_t *)fd;
	if (!size_table || !length) return NULL;

	*length = 0;

	size_t total_size = 0;
	HashmapIter *iter = hashmap_iter_new(size_table);
	while(hashmap_iter_valid(iter)) {
		FilePaths **file_paths;
		hashmap_iter_next(iter, NULL, (void **)&file_paths);
		total_size += file_paths_size(*file_paths);
	} hashmap_iter_free(iter);

	char **paths = calloc(total_size, sizeof(char *));

	size_t index = 0;
	iter = hashmap_iter_new(size_table);
	while (hashmap_iter_valid(iter)) {
		FilePaths **file_paths;
		hashmap_iter_next(iter, NULL, (void **)&file_paths);

		size_t size = 0;
		file_paths_fill(*file_paths, (const char **)paths + index, &size);
		index += size;
	} hashmap_iter_free(iter);

	*length = total_size;
	return paths;
}
