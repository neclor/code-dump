#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

#include "hashmap_entry.h"
#include "hashmap.h"

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
