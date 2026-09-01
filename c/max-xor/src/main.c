#include "trie.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static uint32_t rand_u32(void) {
	return ((uint32_t)rand() & 0xFF) << 24 |
	       ((uint32_t)rand() & 0xFF) << 16 |
	       ((uint32_t)rand() & 0xFF) << 8 |
	       ((uint32_t)rand() & 0xFF);
}

uint32_t *random_array(size_t count) {
	if (count == 0) return NULL;

	uint32_t *nums = malloc(count * sizeof(uint32_t));
	if (nums == NULL) return NULL;

	for (size_t i = 0; i < count; i++) {
		nums[i] = rand_u32();
	}
	return nums;
}


uint32_t array_max_xor(const uint32_t *nums, size_t count) {
	if (count < 2) {
		return 0;
	}

	Trie *trie = trie_create();
	uint32_t best = 0;

	for (size_t i = 0; i < count; i++) {
		trie_add(trie, nums[i]);
	}

	for (size_t i = 0; i < count; i++) {
		uint32_t oppos = trie_max_xor_partner(trie, nums[i]);
		uint32_t num = nums[i] ^ oppos;
		if (num > best) {
			best = num;
		}
	}

	trie_free(trie);
	return best;
}

static double measure_max_xor(const uint32_t *nums, size_t count) {
	clock_t start = clock();
	uint32_t result = array_max_xor(nums, count);
	clock_t end = clock();

	double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("count = %9lu, max xor = %10u, time = %8.2f ms\n", (unsigned long)count, result, ms);
	return ms;
}

int main(void) {
	srand((unsigned)time(NULL));

	size_t sizes[] = {10000, 100000, 1000000, 10000000, 100000000};
	for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
		uint32_t *nums = random_array(sizes[s]);
		if (nums == NULL) exit(EXIT_FAILURE);

		measure_max_xor(nums, sizes[s]);
		free(nums);
	}
	return 0;
}
