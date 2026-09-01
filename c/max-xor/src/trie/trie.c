#include "trie.h"

#include <stdlib.h>

#define TRIE_BITS 32

typedef struct Node_t {
	struct Node_t *child[2];
} Node;

struct Trie_t {
	Node *root;
};

static Node *node_create(void) {
	Node *node = calloc(1, sizeof(Node));
	if (node == NULL) exit(EXIT_FAILURE);

	return node;
}

static void node_free(Node *node) {
	if (node == NULL) return;

	node_free(node->child[0]);
	node_free(node->child[1]);
	free(node);
}

Trie *trie_create(void) {
	Trie *trie = malloc(sizeof(Trie));
	if (trie == NULL) exit(EXIT_FAILURE);

	trie->root = node_create();
	return trie;
}

void trie_free(Trie *trie) {
	if (trie == NULL) return;

	node_free(trie->root);
	free(trie);
}

void trie_add(Trie *trie, uint32_t num) {
	Node *node = trie->root;
	for (int i = TRIE_BITS - 1; i >= 0; i--) {
		int digit = (num >> i) & 1u;
		if (node->child[digit] == NULL) {
			node->child[digit] = node_create();
		}
		node = node->child[digit];
	}
}

uint32_t trie_max_xor_partner(const Trie *trie, uint32_t num) {
	const Node *node = trie->root;
	uint32_t best = 0;
	for (int i = TRIE_BITS - 1; i >= 0; i--) {
		int digit = (num >> i) & 1u;
		int oppos = digit ^ 1;

		if (node->child[oppos] != NULL) {
			best |= (uint32_t)oppos << i;
			node = node->child[oppos];

		} else {
			best |= (uint32_t)digit << i;
			node = node->child[digit];
		}
	}
	return best;
}

