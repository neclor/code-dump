#ifndef HASH_KEY_FUNCTIONS_H
#define HASH_KEY_FUNCTIONS_H

#include <stdlib.h>
#include <stdbool.h>

typedef size_t (*GetHashFunction)(const void *key);
typedef bool (*KeysEqualFunction)(const void *key_a, const void *key_b);

#endif // HASH_KEY_FUNCTIONS_H
