#ifndef HASHSET_H
#define HASHSET_H

#include <stdlib.h>
#include <stdbool.h>

#include "hash_key_functions.h"

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
