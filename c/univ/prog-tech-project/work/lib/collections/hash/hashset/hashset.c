#include <stdlib.h>
#include <stdbool.h>

#include "hashmap.h"
#include "hashset.h"

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
