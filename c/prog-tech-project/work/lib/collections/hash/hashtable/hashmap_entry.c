#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hashmap_entry.h"

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
