#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

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
