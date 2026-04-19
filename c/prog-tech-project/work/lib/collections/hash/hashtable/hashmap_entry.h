#ifndef HASHMAP_ENTRY_H
#define HASHMAP_ENTRY_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hashmap.h"

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
