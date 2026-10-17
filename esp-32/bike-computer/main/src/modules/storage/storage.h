#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>

void storage_init(void);
bool storage_read(const char* key, void* data, size_t size);
void storage_write(const char* key, const void* data, size_t size);

#endif // STORAGE_H
