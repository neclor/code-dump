#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"

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
