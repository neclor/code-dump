/* Auto-generated - do not edit manually. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "filededup.h"

/* ===== lib/collections/vector/vector.h ===== */

#ifndef VECTOR_H
#define VECTOR_H
typedef struct Vector_t Vector;
typedef struct VectorIter_t VectorIter;

/* ======= Vector ======= */

Vector *vector_new(size_t type_size);

void vector_free(Vector *self);

size_t vector_type_size(const Vector *self);

size_t vector_capacity(const Vector *self);

size_t vector_size(const Vector *self);

bool vector_is_empty(const Vector *self);

void *vector_at(const Vector *self, size_t index);

void *vector_front(const Vector *self);

void *vector_back(const Vector *self);

void *vector_set(Vector *self, size_t index, const void *value);

void *vector_insert(Vector *self, size_t index, const void *value);

void *vector_push_back(Vector *self, const void *value);

void vector_remove_at(Vector *self, size_t index);

void vector_pop_back(Vector *self);

bool vector_reserve(Vector *self, size_t new_capacity);

/* ======= VectorIter ======= */

VectorIter *vector_iter_new(Vector *vector);

void vector_iter_free(VectorIter *self);

size_t vector_iter_index(const VectorIter *self);

bool vector_iter_valid(const VectorIter *self);

void *vector_iter_at(const VectorIter *self);

void *vector_iter_next(VectorIter *self);

void *vector_iter_set(const VectorIter *self, const void *value);

#endif // VECTOR_H

/* ===== lib/collections/hash/hash_key_functions.h ===== */

#ifndef HASH_KEY_FUNCTIONS_H
#define HASH_KEY_FUNCTIONS_H
typedef size_t (*GetHashFunction)(const void *key);
typedef bool (*KeysEqualFunction)(const void *key_a, const void *key_b);

#endif // HASH_KEY_FUNCTIONS_H

/* ===== lib/collections/hash/hashtable/hashmap_entry.h ===== */

#ifndef HASHMAP_ENTRY_H
#define HASHMAP_ENTRY_H
typedef struct HashmapEntry_t {
	bool is_empty;
	size_t next;
	size_t hash;
} HashmapEntry;

static inline size_t sizeof_hashmap_entry(size_t key_size, size_t value_size) { return sizeof(HashmapEntry) + key_size + value_size; }

static inline void *hashmap_entry_key(const HashmapEntry *self) { return (uint8_t *)self + sizeof(HashmapEntry); }

static inline void *hashmap_entry_value(const HashmapEntry *self, size_t key_size) { return (uint8_t *)hashmap_entry_key(self) + key_size; }

HashmapEntry *hashmap_entry_init(HashmapEntry *self, size_t next, size_t hash, const void *key, size_t key_size, const void *value, size_t value_size);

void *hashmap_entry_set_key(HashmapEntry *self, const void *key, size_t key_size);

void *hashmap_entry_set_value(HashmapEntry *self, size_t key_size, const void *value, size_t value_size);

#endif // HASHMAP_ENTRY_H

/* ===== lib/collections/hash/hashtable/hashmap.h ===== */

#ifndef HASHMAP_H
#define HASHMAP_H
typedef struct Hashmap_t Hashmap;
typedef struct HashmapIter_t HashmapIter;

/* ======= Hashmap ======= */

Hashmap *hashmap_new(size_t key_size, size_t value_size, GetHashFunction key_hash, KeysEqualFunction keys_equal);

void hashmap_free(Hashmap *self);

size_t hashmap_capacity(const Hashmap *self);

size_t hashmap_size(const Hashmap *self);

bool hashmap_is_empty(const Hashmap *self);

bool hashmap_contains(const Hashmap *self, const void *key);

void *hashmap_at(const Hashmap *self, const void *key);

void *hashmap_set(Hashmap *self, const void *key, const void *value);

void *hashmap_insert(Hashmap *self, const void *key, const void *value);

void *hashmap_at_or_insert(Hashmap *self, const void *key, const void *value);

bool hashmap_remove(Hashmap *self, const void *key);

bool hashmap_reserve(Hashmap *self, size_t new_capacity);

bool hashmap_defragment(Hashmap *self);

/* ======= HashmapIter ======= */

HashmapIter *hashmap_iter_new(Hashmap *hashmap);

void hashmap_iter_free(HashmapIter *self);

bool hashmap_iter_valid(const HashmapIter *self);

bool hashmap_iter_at(const HashmapIter *self, const void **out_key, void **out_value);

bool hashmap_iter_next(HashmapIter *self, const void **out_key, void **out_value);

void *hashmap_iter_set(const HashmapIter *self, const void *value);

#endif // HASHMAP_H

/* ===== lib/collections/hash/hashset/hashset.h ===== */

#ifndef HASHSET_H
#define HASHSET_H
typedef struct Hashset_t Hashset;
typedef struct HashsetIter_t HashsetIter;

/* ======= Hashset ======= */

Hashset *hashset_new(size_t key_size, GetHashFunction get_hash, KeysEqualFunction keys_equal);

void hashset_free(Hashset *self);

size_t hashset_capacity(const Hashset *self);

size_t hashset_size(const Hashset *self);

bool hashset_is_empty(const Hashset *self);

bool hashset_contains(Hashset *self, const void *key);

bool hashset_add(Hashset *self, const void *key);

bool hashset_remove(Hashset *self, const void *key);

bool hashset_reserve(Hashset *self, size_t new_capacity);

bool hashset_defragment(Hashset *self);

/* ======= HashsetIter ======= */

HashsetIter *hashset_iter_new(Hashset *Hashset);

void hashset_iter_free(HashsetIter *self);

bool hashset_iter_valid(const HashsetIter *self);

bool hashset_iter_at(const HashsetIter *self, const void **out_key);

bool hashset_iter_next(HashsetIter *self, const void **out_key);

#endif // HASHSET_H

/* ===== src/utils/hash_functions/hash_functions.h ===== */

#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H
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

/* ===== src/utils/file_utils/file_utils.h ===== */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H
/* Preserves file position */
long file_size(FILE *file);

/* Compares content. Preserves file positions */
bool file_equal(FILE *a, FILE *b);

bool file_equal_by_path(const char *path_a, const char *path_b);

bool file_equal_by_path_ptr(const char **path_ptr_a, const char **path_ptr_b);

#endif // FILE_UTILS_H

/* ===== src/file_paths/file_paths.h ===== */

#ifndef FILE_PATHS_H
#define FILE_PATHS_H
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

/* ===== lib/collections/vector/vector.c ===== */

#define VECTOR_INITIAL_CAPACITY (8)
#define VECTOR_RESIZE_FACTOR (2)

struct Vector_t {
	size_t type_size;

	size_t size;
	size_t capacity;

	void *data;
};

struct VectorIter_t{
    Vector *vector;
    size_t index;
};

/* ======= Vector public functions ======= */

Vector *vector_new(size_t type_size) {
	Vector *self = malloc(sizeof(Vector));
	if (!self) return NULL;

	self->capacity = 0;
	self->size = 0;
	self->type_size = type_size;
	self->data = NULL;

	return self;
}

void vector_free(Vector *self) {
	if (!self) return;
	free(self->data);
	free(self);
}

size_t vector_type_size(const Vector *self) { return self ? self->type_size : 0; }

size_t vector_capacity(const Vector *self) { return self ? self->capacity : 0; }

size_t vector_size(const Vector *self) { return self ? self->size : 0; }

bool vector_is_empty(const Vector *self) { return self ? self->size == 0 : true; }

void *vector_at(const Vector *self, size_t index) { return (!self || index >= self->size) ? NULL : (uint8_t *)self->data + (index * self->type_size); }

void *vector_front(const Vector *self) { return self ? vector_at(self, 0) : NULL; }

void *vector_back(const Vector *self) { return (self && self->size != 0) ? vector_at(self, self->size - 1) : NULL; }

void *vector_set(Vector *self, size_t index, const void *value) {
	if (!self || index >= self->size) return NULL;

	if (!value) return memset(vector_at(self, index), 0, self->type_size);
	return memcpy(vector_at(self, index), value, self->type_size);
}

void *vector_insert(Vector *self, size_t index, const void *value) {
	if (!self || index > self->size) return NULL;

	if (self->capacity <= self->size) {
		size_t new_capacity = self->capacity == 0 ? VECTOR_INITIAL_CAPACITY : self->capacity * VECTOR_RESIZE_FACTOR;
		if (!vector_reserve(self, new_capacity)) return NULL;
	}

	self->size++;

	void *dest = vector_at(self, index + 1);
	void *src = vector_at(self, index);
	if (dest && src) memmove(dest, src, (self->size - index - 1) * self->type_size);

	return vector_set(self, index, value);
}

void *vector_push_back(Vector *self, const void *value) { return self ? vector_insert(self, self->size, value) : NULL; }

void vector_remove_at(Vector *self, size_t index) {
	if (!self || index >= self->size) return;

	memmove(vector_at(self, index), vector_at(self, index + 1), (self->size - index - 1) * self->type_size);
	self->size--;
}

void vector_pop_back(Vector *self) {
	if (!self || self->size == 0) return;

	vector_remove_at(self, self->size - 1);
}

bool vector_reserve(Vector *self, size_t new_capacity) {
	if (!self || new_capacity <= self->capacity) return false;

	void *new_data = realloc(self->data, new_capacity * self->type_size);
	if (!new_data) return false;

	self->data = new_data;
	self->capacity = new_capacity;
	return true;
}

/* ======= VectorIter public functions ======= */

VectorIter *vector_iter_new(Vector *vector) {
	if (!vector) return NULL;

	VectorIter *self = malloc(sizeof(VectorIter));
	if (!self) return NULL;

	self->vector = vector;
	self->index = 0;

	return self;
}

void vector_iter_free(VectorIter *self) { free(self); }

size_t vector_iter_index(const VectorIter *self) { return self ? self->index : 0; }

bool vector_iter_valid(const VectorIter *self) { return self && self->index < vector_size(self->vector); }

void *vector_iter_at(const VectorIter *self) { return vector_iter_valid(self) ? vector_at(self->vector, self->index) : NULL; }

void *vector_iter_next(VectorIter *self) {
	if (!vector_iter_valid(self)) return NULL;

	void *value = vector_iter_at(self);
	self->index++;
	return value;
}

void *vector_iter_set(const VectorIter *self, const void *value) { return vector_iter_valid(self) ? vector_set(self->vector, self->index, value) : NULL; }

/* ===== lib/collections/hash/hashtable/hashmap_entry.c ===== */

/* ======= Public functions ======= */

HashmapEntry *hashmap_entry_init(HashmapEntry *self, size_t next, size_t hash, const void *key, size_t key_size, const void *value, size_t value_size) {
	self->is_empty = false;
	self->next = next;
	self->hash = hash;
	hashmap_entry_set_key(self, key, key_size);
	hashmap_entry_set_value(self, key_size, value, value_size);
	return self;
}

void *hashmap_entry_set_key(HashmapEntry *self, const void *key, size_t key_size) {
	return memcpy(hashmap_entry_key(self), key, key_size);
}

void *hashmap_entry_set_value(HashmapEntry *self, size_t key_size, const void *value, size_t value_size) {
	if (!value) return memset(hashmap_entry_value(self, key_size), 0, value_size);
	return memcpy(hashmap_entry_value(self, key_size), value, value_size);
}

/* ===== lib/collections/hash/hashtable/hashmap.c ===== */

#define HASHMAP_INVALID_INDEX ((size_t) - 1)

#define HASHMAP_INITIAL_CAPACITY (32)
#define HASHMAP_RESIZE_FACTOR (2)
#define HASHMAP_LOAD_FACTOR (0.75)
#define HASHMAP_ENTRY_DENSITY_FACTOR (2)

struct Hashmap_t {
	size_t free_index;

	size_t size;
	size_t capacity;

	size_t *indices;
	Vector *entries;

	size_t key_size;
	size_t value_size;

	GetHashFunction key_hash;
	KeysEqualFunction keys_equal;
};

struct HashmapIter_t {
	Hashmap *hashmap;
	size_t index;
};

/* ======= Hashmap static functions prototypes ======= */

static HashmapEntry *hashmap_find_entry(const Hashmap *self, size_t index, size_t hash, const void *key);
static HashmapEntry *hashmap_internal_insert(Hashmap *self, size_t hash, const void *key, const void *value);

/* ======= Hashmap public functions ======= */

Hashmap *hashmap_new(size_t key_size, size_t value_size, GetHashFunction key_hash, KeysEqualFunction keys_equal) {
	if (!key_hash || !keys_equal) return NULL;

	Hashmap *self = malloc(sizeof(Hashmap));
	size_t *indices = calloc(HASHMAP_INITIAL_CAPACITY, sizeof(size_t));
	Vector *entries = vector_new(sizeof_hashmap_entry(key_size, value_size));
	if (!self || !indices || !entries) {
		free(self);
		free(indices);
		vector_free(entries);
		return NULL;
	}

	for (size_t i = 0; i < HASHMAP_INITIAL_CAPACITY; i++) {
		indices[i] = HASHMAP_INVALID_INDEX;
	}

	self->free_index = HASHMAP_INVALID_INDEX;

	self->size = 0;
	self->capacity = HASHMAP_INITIAL_CAPACITY;
	self->indices = indices;
	self->entries = entries;

	self->key_size = key_size;
	self->value_size = value_size;

	self->key_hash = key_hash;
	self->keys_equal = keys_equal;

	return self;
}

void hashmap_free(Hashmap *self) {
	if (!self) return;

	free(self->indices);
	vector_free(self->entries);
	free(self);
}

size_t hashmap_capacity(const Hashmap *self) { return self ? self->capacity : 0; }

size_t hashmap_size(const Hashmap *self) { return self ? self->size : 0; }

bool hashmap_is_empty(const Hashmap *self) { return !self || self->size == 0; }

bool hashmap_contains(const Hashmap *self, const void *key) {
	if (!self || !key) return false;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;
	return hashmap_find_entry(self, index, hash, key) != NULL;
}

void *hashmap_at(const Hashmap *self, const void *key) {
	if (!self || !key) return NULL;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;
	HashmapEntry *entry = hashmap_find_entry(self, index, hash, key);
	return entry ? hashmap_entry_value(entry, self->key_size) : NULL;
}

void *hashmap_set(Hashmap *self, const void *key, const void *value) {
	if (!self || !key) return NULL;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;
	HashmapEntry *entry = hashmap_find_entry(self, index, hash, key);
	if (entry) return hashmap_entry_set_value(entry, self->key_size, value, self->value_size);

	entry = hashmap_internal_insert(self, hash, key, value);
	return entry ? hashmap_entry_value(entry, self->key_size) : NULL;
}

void *hashmap_insert(Hashmap *self, const void *key, const void *value) {
	if (!self || !key) return NULL;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;
	if(hashmap_find_entry(self, index, hash, key)) return NULL;

	HashmapEntry *entry = hashmap_internal_insert(self, hash, key, value);
	return entry ? hashmap_entry_value(entry, self->key_size) : NULL;
}

void *hashmap_at_or_insert(Hashmap *self, const void *key, const void *value) {
	if (!self || !key) return NULL;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;
	HashmapEntry *entry = hashmap_find_entry(self, index, hash, key);
	if (entry) return hashmap_entry_value(entry, self->key_size);

	entry = hashmap_internal_insert(self, hash, key, value);
	return entry ? hashmap_entry_value(entry, self->key_size) : NULL;
}

bool hashmap_remove(Hashmap *self, const void *key) {
	if (!self || !key) return false;

	size_t hash = self->key_hash(key);
	size_t index = hash % self->capacity;

	HashmapEntry *previous = NULL;
	HashmapEntry *found = NULL;

	size_t i = self->indices[index];
	while (i != HASHMAP_INVALID_INDEX) {
		HashmapEntry *entry = vector_at(self->entries, i);
		if(entry->hash == hash && self->keys_equal(hashmap_entry_key(entry), key)) {
			found = entry;
			break;
		}
		previous = entry;
		i = entry->next;
	}
	if (!found) return false;

	if (previous) previous->next = found->next;
	else self->indices[index] = found->next;

	found->is_empty = true;
	found->next = self->free_index;
	self->free_index = i;
	self->size--;

	if (self->size * HASHMAP_ENTRY_DENSITY_FACTOR <= vector_size(self->entries)) hashmap_defragment(self);

	return true;
}

bool hashmap_reserve(Hashmap *self, size_t new_capacity) {
	if (!self || new_capacity <= self->capacity) return false;

	size_t *new_indices = realloc(self->indices, new_capacity * sizeof(size_t));
	if (!new_indices) {
		return false;
	}

	size_t old_capacity	= self->capacity;
	self->capacity = new_capacity;
	self->indices = new_indices;

	if (hashmap_defragment(self)) return true;

	size_t *old_indices = realloc(self->indices, old_capacity * sizeof(size_t));
	if (old_indices) {
		self->indices = old_indices;
	}
	self->capacity = old_capacity;

	return false;
}

bool hashmap_defragment(Hashmap *self) {
	if (!self) return false;

	Vector *entries = self->entries;

	VectorIter *iter = vector_iter_new(entries);
	Vector *new_entries = vector_new(sizeof_hashmap_entry(self->key_size, self->value_size));

	size_t new_entries_capacity = self->size * HASHMAP_RESIZE_FACTOR;

	if (!iter || !new_entries || (new_entries_capacity != 0 && !vector_reserve(new_entries, new_entries_capacity))) {
		vector_iter_free(iter);
		vector_free(new_entries);
		return false;
	}

	size_t capacity = self->capacity;
	size_t *indices = self->indices;
	for (size_t i = 0; i < capacity; i++) {
		indices[i] = HASHMAP_INVALID_INDEX;
	}

	while (vector_iter_valid(iter)) {
		HashmapEntry *entry = (HashmapEntry *)vector_iter_next(iter);
		if (entry->is_empty) continue;

		size_t index = entry->hash % capacity;
		size_t next_entry_index = indices[index];

		entry = vector_push_back(new_entries, entry);
		entry->next = next_entry_index;
		indices[index] = vector_size(new_entries) - 1;
	} vector_iter_free(iter);
	vector_free(entries);

	self->free_index = HASHMAP_INVALID_INDEX;
	self->entries = new_entries;

	return true;
}

/* ======= Hashmap static functions ======= */

static HashmapEntry *hashmap_find_entry(const Hashmap *self, size_t index, size_t hash, const void *key) {
	for (size_t i = self->indices[index]; i != HASHMAP_INVALID_INDEX;) {
		HashmapEntry *entry = vector_at(self->entries, i);
		if((entry->hash == hash) && self->keys_equal(hashmap_entry_key(entry), key)) return entry;
		i = entry->next;
	}
	return NULL;
}

static HashmapEntry *hashmap_internal_insert(Hashmap *self, size_t hash, const void *key, const void *value) {
	if (self->capacity * HASHMAP_LOAD_FACTOR <= self->size) {
		if (!hashmap_reserve(self, self->capacity * HASHMAP_RESIZE_FACTOR)) return NULL;
	}

	size_t index = hash % self->capacity;
	size_t *indices = self->indices;
	Vector *entries = self->entries;

	size_t entry_index;
	HashmapEntry *entry = NULL;

	if (self->free_index == HASHMAP_INVALID_INDEX) {
		entry_index = vector_size(entries);
		entry = vector_push_back(entries, NULL);
		if (!entry) return NULL;

	} else {
		entry_index = self->free_index;
		entry = vector_at(entries, entry_index);
		self->free_index = entry->next;
	}

	size_t next_entry_index = indices[index];
	indices[index] = entry_index;
	self->size++;

	return hashmap_entry_init(entry, next_entry_index, hash, key, self->key_size, value, self->value_size);
}

/* ======= HashmapIter static functions prototypes ======= */

static void hashmap_iter_skip_empty(HashmapIter *self);

/* ======= HashmapIter public functions ======= */

HashmapIter *hashmap_iter_new(Hashmap *hashmap) {
	if (!hashmap) return NULL;

	HashmapIter *self = malloc(sizeof(HashmapIter));
	if (!self) return NULL;

	self->hashmap = hashmap;
	self->index = 0;

	hashmap_iter_skip_empty(self);

	return self;
}

void hashmap_iter_free(HashmapIter *self) { free(self); }
bool hashmap_iter_valid(const HashmapIter *self) { return self && self->index < vector_size(self->hashmap->entries); }

bool hashmap_iter_at(const HashmapIter *self, const void **out_key, void **out_value) {
	if (!hashmap_iter_valid(self)) return false;

	HashmapEntry *entry = vector_at(self->hashmap->entries, self->index);
	if (entry->is_empty) return false;

	if (out_key) *out_key = hashmap_entry_key(entry);
	if (out_value) *out_value = hashmap_entry_value(entry, self->hashmap->key_size);

	return true;
}

bool hashmap_iter_next(HashmapIter *self, const void **out_key, void **out_value) {
	if (!hashmap_iter_valid(self)) return false;

	HashmapEntry *entry = vector_at(self->hashmap->entries, self->index);
	if (out_key) *out_key = hashmap_entry_key(entry);
	if (out_value) *out_value = hashmap_entry_value(entry, self->hashmap->key_size);

	self->index++;
	hashmap_iter_skip_empty(self);

	return true;
}

void *hashmap_iter_set(const HashmapIter *self, const void *value) {
	if (!hashmap_iter_valid(self)) return NULL;

	HashmapEntry *entry = vector_at(self->hashmap->entries, self->index);
	return hashmap_entry_set_value(entry, self->hashmap->key_size, value, self->hashmap->value_size);
}

/* ======= HashmapIter static functions ======= */

static void hashmap_iter_skip_empty(HashmapIter *self) {
	while (self->index < vector_size(self->hashmap->entries)) {
		HashmapEntry *entry = vector_at(self->hashmap->entries, self->index);
		if (!entry->is_empty) break;
		self->index++;
	}
}

/* ===== lib/collections/hash/hashset/hashset.c ===== */

typedef struct Hashmap Hashset_t;
typedef struct HashmapIter HashsetIter_t;

/* ======= Hashset public functions ======= */

Hashset *hashset_new(size_t key_size, GetHashFunction get_hash, KeysEqualFunction keys_equal) { return (Hashset *)hashmap_new(key_size, 0, get_hash, keys_equal); }

void hashset_free(Hashset *self) { hashmap_free((Hashmap *)self); }

size_t hashset_capacity(const Hashset *self) { return hashmap_capacity((Hashmap *)self); }

size_t hashset_size(const Hashset *self) { return hashmap_size((Hashmap *)self); }

bool hashset_is_empty(const Hashset *self) { return hashmap_is_empty((Hashmap *)self); }

bool hashset_contains(Hashset *self, const void *key) { return hashmap_contains((Hashmap *)self, key); }

bool hashset_add(Hashset *self, const void *key) { return hashmap_insert((Hashmap *)self, key, NULL) != NULL; }

bool hashset_remove(Hashset *self, const void *key) { return hashmap_remove((Hashmap *)self, key); }

bool hashset_reserve(Hashset *self, size_t new_capacity) { return hashmap_reserve((Hashmap *)self, new_capacity); }

bool hashset_defragment(Hashset *self) { return hashmap_defragment((Hashmap *)self); }

/* ======= HashsetIter public functions ======= */

HashsetIter *hashset_iter_new(Hashset *hashset) { return (HashsetIter *)hashmap_iter_new((Hashmap *)hashset); }

void hashset_iter_free(HashsetIter *self) { hashmap_iter_free((HashmapIter *)self); }

bool hashset_iter_valid(const HashsetIter *self) { return hashmap_iter_valid((HashmapIter *)self); }

bool hashset_iter_at(const HashsetIter *self, const void **out_key) { return hashmap_iter_at((HashmapIter *)self, out_key, NULL); }

bool hashset_iter_next(HashsetIter *self, const void **out_key) { return hashmap_iter_next((HashmapIter *)self, out_key, NULL); }

/* ===== src/utils/hash_functions/hash_functions.c ===== */

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

/* ===== src/utils/file_utils/file_utils.c ===== */

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

/* ===== src/file_paths/file_paths.c ===== */

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

/* ===== src/filededup.c ===== */

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
