#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "hashmap.h"
#include "hashset.h"

#include "file_utils.h"
#include "hash_functions.h"

#include "file_paths.h"

struct FilePaths_t {
	bool is_single;
	union {
		const char *single_path;
		Hashmap *path_sets;
	};
};

/* ======= Prototypes of static functions ======= */

static bool file_paths_internal_add_or_create(Hashmap *path_sets, const char *path);

/* ======= Public functions ======= */

FilePaths *file_paths_new(const char *path) {
	if (!path) return NULL;

	FilePaths *self = malloc(sizeof(FilePaths));
	if (!self) return NULL;

	self->is_single = true;
	self->single_path = path;

	return self;
}

void file_paths_free(FilePaths *self) {
	if (!self) return;

	if (self->is_single) {
		free((void *)self->single_path);
		free(self);
		return;
	}

	Hashmap *path_sets = self->path_sets;
	if (!path_sets) {
		free(self);
		return;
	}

	HashmapIter *iter = hashmap_iter_new(path_sets);
	while(hashmap_iter_valid(iter)) {
		Hashset **path_set;
		hashmap_iter_next(iter, NULL, (void **)&path_set);

		HashsetIter *set_iter = hashset_iter_new(*path_set);
		while (hashset_iter_valid(set_iter)) {
			const char **path;
			hashset_iter_next(set_iter, (const void **)&path);
			free((void *)*path);
		} hashset_iter_free(set_iter);

		if (path_set) hashset_free(*path_set);
	} hashmap_iter_free(iter);

	hashmap_free(path_sets);
	free(self);
}

bool file_paths_add(FilePaths *self, const char *path) {
	if (!self || !path) return false;

	if (self->is_single) {
		if (strcmp(self->single_path, path) == 0) return false;

		const char *existing_path = self->single_path;

		Hashmap *path_sets = hashmap_new(sizeof(char *), sizeof(Hashset *), (GetHashFunction)file_hash_by_path_ptr, (KeysEqualFunction)file_equal_by_path_ptr);
		if (!path_sets) return false;

		if(!file_paths_internal_add_or_create(path_sets, existing_path)) {
			hashmap_free(path_sets);
			return false;
		}
		self->path_sets = path_sets;
		self->is_single = false;
	}

	return file_paths_internal_add_or_create(self->path_sets, path);
}

size_t file_paths_size(const FilePaths *self) {
	if (!self) return 0;

	if (self->is_single) return 0;

	Hashmap *path_sets = self->path_sets;
	if (!path_sets) return 0;

	size_t total_count = 0;
	HashmapIter *iter = hashmap_iter_new(path_sets);
	while(hashmap_iter_valid(iter)) {
		Hashset **path_set;
		hashmap_iter_next(iter, NULL, (void **)&path_set);

		size_t set_size = hashset_size(*path_set);
		if (set_size >= 2)
			total_count += set_size + 1;
	} hashmap_iter_free(iter);

	return total_count;
}

bool file_paths_fill(const FilePaths *self, const char **out_paths, size_t *out_size) {
	if (!self || !out_paths || !out_size) return false;

	if (self->is_single) {
		*out_size = 0;
		return true;
	}

	Hashmap *path_sets = self->path_sets;
	if (!path_sets) return false;

	size_t index = 0;
	HashmapIter *iter = hashmap_iter_new(path_sets);
	while (hashmap_iter_valid(iter)) {
		Hashset **path_set;
		hashmap_iter_next(iter, NULL, (void **)&path_set);

		if (hashset_size(*path_set) < 2) continue;

		HashsetIter *set_iter = hashset_iter_new(*path_set);
		while (hashset_iter_valid(set_iter)) {
			const char **path;
			hashset_iter_next(set_iter, (const void **)&path);
			out_paths[index++] = *path;
		} hashset_iter_free(set_iter);

		out_paths[index++] = NULL;
	} hashmap_iter_free(iter);

	*out_size = index;
	return true;
}

/* ======= Static functions ======= */

static bool file_paths_internal_add_or_create(Hashmap *path_sets, const char *path) {
	Hashset *path_set = NULL;
	Hashset **path_set_ptr = hashmap_at_or_insert(path_sets, &path, &path_set);
	if (!path_set_ptr) return false;

	path_set = *path_set_ptr;
	if (!path_set) {
		path_set = hashset_new(sizeof(char *), (GetHashFunction)string_hash_by_ptr, (KeysEqualFunction)string_equal_by_ptr);
		if (!path_set) return false;

		*path_set_ptr = path_set;
	}

	return hashset_add(path_set, &path);
}
