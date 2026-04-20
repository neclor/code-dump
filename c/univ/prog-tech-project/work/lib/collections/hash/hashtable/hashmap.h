#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>

#include "hash_key_functions.h"

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
