#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>

typedef struct Trie_t Trie;

Trie *trie_create(void);

void trie_free(Trie *trie);

void trie_add(Trie *trie, uint32_t num);

uint32_t trie_max_xor_partner(const Trie *trie, uint32_t num);

#endif // TRIE_H
