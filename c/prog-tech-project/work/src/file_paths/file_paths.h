#ifndef FILE_PATHS_H
#define FILE_PATHS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct FilePaths_t FilePaths;

/* Takes ownership of path (must be heap-allocated) */
FilePaths *file_paths_new(const char *path);

/* Frees the struct and all owned path strings */
void file_paths_free(FilePaths *self);

/* Takes ownership of path. Returns false if duplicate or error */
bool file_paths_add(FilePaths *self, const char *path);

/* Returns total entry count including NULL separators between groups */
size_t file_paths_size(const FilePaths *self);

/* Fills out_paths with paths separated by NULL between groups */
bool file_paths_fill(const FilePaths *self, const char **out_paths, size_t *out_size);

#endif // FILE_PATHS_H
