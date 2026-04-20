#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

#include "hashmap_test.h"

static inline size_t int_hash_by_ptr(const void *key) {
	return (size_t)(*(const int *)key);
}

static inline bool int_equal_by_ptr(const void *a, const void *b) {
	return *(const int *)a == *(const int *)b;
}

static int tests_passed = 0;
static int tests_failed = 0;

static void check(bool condition, const char *name) {
	if (condition) {
		tests_passed++;
		return;
	}

	tests_failed++;
	printf("FAIL: %s\n", name);
}

/* ======= Tests ======= */

static void test_new_free(void) {
	printf("test_new_free\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	check(m != NULL, "hashmap_new returns non-NULL");
	check(hashmap_size(m) == 0, "new map size is 0");
	check(hashmap_is_empty(m), "new map is empty");
	check(hashmap_capacity(m) > 0, "new map capacity > 0");
	hashmap_free(m);

	check(hashmap_new(sizeof(int), sizeof(int), NULL, int_equal_by_ptr) == NULL, "NULL hash func rejected");
	check(hashmap_new(sizeof(int), sizeof(int), int_hash_by_ptr, NULL) == NULL, "NULL equal func rejected");
}

static void test_insert_contains_at(void) {
	printf("test_insert_contains_at\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 42, v = 100;

	void *ret = hashmap_insert(m, &k, &v);
	check(ret != NULL, "insert returns non-NULL");
	check(*(int *)ret == v, "insert returns correct value");
	check(hashmap_size(m) == 1, "size is 1 after insert");
	check(!hashmap_is_empty(m), "map not empty after insert");
	check(hashmap_contains(m, &k), "contains inserted key");

	void *val = hashmap_at(m, &k);
	check(val != NULL, "at returns non-NULL");
	check(*(int *)val == v, "at returns correct value");

	int k2 = 99;
	check(!hashmap_contains(m, &k2), "does not contain absent key");
	check(hashmap_at(m, &k2) == NULL, "at returns NULL for absent key");

	check(hashmap_insert(m, &k, &v) == NULL, "duplicate insert returns NULL");
	check(hashmap_size(m) == 1, "size unchanged after duplicate insert");

	hashmap_free(m);
}

static void test_set(void) {
	printf("test_set\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 1, v1 = 10, v2 = 20;

	void *ret = hashmap_set(m, &k, &v1);
	check(ret != NULL && *(int *)ret == v1, "set inserts new key");;
	check(hashmap_size(m) == 1, "size 1 after set");

	ret = hashmap_set(m, &k, &v2);
	check(ret != NULL && *(int *)ret == v2, "set overwrites value");
	check(hashmap_size(m) == 1, "size unchanged after overwrite");
	check(*(int *)hashmap_at(m, &k) == v2, "at reflects overwritten value");

	hashmap_free(m);
}

static void test_remove(void) {
	printf("test_remove\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 5, v = 50;
	hashmap_insert(m, &k, &v);

	check(hashmap_remove(m, &k), "remove existing returns true");
	check(hashmap_size(m) == 0, "size 0 after remove");
	check(!hashmap_contains(m, &k), "key gone after remove");
	check(hashmap_at(m, &k) == NULL, "at NULL after remove");

	check(!hashmap_remove(m, &k), "remove absent returns false");

	int absent = 999;
	check(!hashmap_remove(m, &absent), "remove never-inserted returns false");

	hashmap_free(m);
}

static void test_null_value(void) {
	printf("test_null_value\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 7;

	void *ret = hashmap_insert(m, &k, NULL);
	check(ret != NULL, "insert with NULL value succeeds");
	check(*(int *)ret == 0, "NULL value stored as zero");
	check(hashmap_contains(m, &k), "key present after NULL-value insert");

	hashmap_free(m);
}

static void test_many_inserts_and_resize(void) {
	printf("test_many_inserts_and_resize\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	size_t init_cap = hashmap_capacity(m);
	int n = 200;

	for (int i = 0; i < n; i++) {
		int v = i * 10;
		hashmap_insert(m, &i, &v);
	}
	check(hashmap_size(m) == (size_t)n, "size correct after bulk insert");
	check(hashmap_capacity(m) > init_cap, "capacity grew");

	for (int i = 0; i < n; i++) {
		check(hashmap_contains(m, &i), "contains each key after resize");
		check(*(int *)hashmap_at(m, &i) == i * 10, "value correct after resize");
	}

	hashmap_free(m);
}

static void test_remove_reinsert(void) {
	printf("test_remove_reinsert\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	for (int i = 0; i < 10; i++) {
		int v = i;
		hashmap_insert(m, &i, &v);
	}

	for (int i = 0; i < 10; i += 2) {
		hashmap_remove(m, &i);
	}
	check(hashmap_size(m) == 5, "size 5 after removing evens");

	for (int i = 0; i < 10; i += 2) {
		int v = i + 100;
		hashmap_insert(m, &i, &v);
	}
	check(hashmap_size(m) == 10, "size 10 after reinserting evens");

	for (int i = 0; i < 10; i += 2) {
		check(*(int *)hashmap_at(m, &i) == i + 100, "reinserted value correct");
	}
	for (int i = 1; i < 10; i += 2) {
		check(*(int *)hashmap_at(m, &i) == i, "untouched odd value correct");
	}

	hashmap_free(m);
}

static void test_hash_collision(void) {
	printf("test_hash_collision\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	size_t cap = hashmap_capacity(m);

	int k1 = 0, k2 = (int)cap, k3 = (int)(cap * 2);
	int v1 = 1, v2 = 2, v3 = 3;
	hashmap_insert(m, &k1, &v1);
	hashmap_insert(m, &k2, &v2);
	hashmap_insert(m, &k3, &v3);

	check(*(int *)hashmap_at(m, &k1) == 1, "collision key1 correct");
	check(*(int *)hashmap_at(m, &k2) == 2, "collision key2 correct");
	check(*(int *)hashmap_at(m, &k3) == 3, "collision key3 correct");

	hashmap_remove(m, &k2);
	check(!hashmap_contains(m, &k2), "middle collision key removed");
	check(*(int *)hashmap_at(m, &k1) == 1, "key1 still accessible");
	check(*(int *)hashmap_at(m, &k3) == 3, "key3 still accessible");

	hashmap_free(m);
}

static void test_reserve(void) {
	printf("test_reserve\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 1, v = 10;
	hashmap_insert(m, &k, &v);

	size_t old_cap = hashmap_capacity(m);
	check(hashmap_reserve(m, old_cap * 4), "reserve succeeds");
	check(hashmap_capacity(m) >= old_cap * 4, "capacity increased");
	check(hashmap_size(m) == 1, "size preserved after reserve");
	check(*(int *)hashmap_at(m, &k) == 10, "value preserved after reserve");

	check(!hashmap_reserve(m, 1), "reserve to smaller fails");

	hashmap_free(m);
}

static void test_defragment(void) {
	printf("test_defragment\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	for (int i = 0; i < 50; i++) {
		int v = i;
		hashmap_insert(m, &i, &v);
	}
	for (int i = 0; i < 50; i += 2) {
		hashmap_remove(m, &i);
	}

	check(hashmap_defragment(m), "defragment succeeds");
	check(hashmap_size(m) == 25, "size preserved after defragment");

	for (int i = 1; i < 50; i += 2) {
		check(hashmap_contains(m, &i), "odd key present after defragment");
		check(*(int *)hashmap_at(m, &i) == i, "odd value correct after defragment");
	}
	for (int i = 0; i < 50; i += 2) {
		check(!hashmap_contains(m, &i), "even key absent after defragment");
	}

	hashmap_free(m);
}

static void test_iterator(void) {
	printf("test_iterator\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	for (int i = 0; i < 10; i++) {
		int v = i * 10;
		hashmap_insert(m, &i, &v);
	}

	HashmapIter *it = hashmap_iter_new(m);
	check(it != NULL, "iter created");
	check(hashmap_iter_valid(it), "iter valid at start");

	int count = 0;
	bool seen[10] = {false};
	const void *out_key;
	void *out_value;

	while (hashmap_iter_next(it, &out_key, &out_value)) {
		int k = *(const int *)out_key;
		int v = *(int *)out_value;
		check(k >= 0 && k < 10, "iter key in range");
		check(v == k * 10, "iter value matches key");
		check(!seen[k], "iter key not seen twice");
		seen[k] = true;
		count++;
	}
	check(count == 10, "iter visited all 10 entries");
	check(!hashmap_iter_valid(it), "iter invalid after exhaustion");

	hashmap_iter_free(it);
	hashmap_free(m);
}

static void test_iter_set(void) {
	printf("test_iter_set\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	int k = 1, v = 10;
	hashmap_insert(m, &k, &v);

	HashmapIter *it = hashmap_iter_new(m);
	int new_v = 999;
	void *ret = hashmap_iter_set(it, &new_v);
	check(ret != NULL && *(int *)ret == 999, "iter_set returns new value");
	check(*(int *)hashmap_at(m, &k) == 999, "value updated via iter_set");

	hashmap_iter_free(it);
	hashmap_free(m);
}

static void test_iter_empty_map(void) {
	printf("test_iter_empty_map\n");

	Hashmap *m = hashmap_new(sizeof(int), sizeof(int), (GetHashFunction)int_hash_by_ptr, (KeysEqualFunction)int_equal_by_ptr);
	HashmapIter *it = hashmap_iter_new(m);
	check(it != NULL, "iter on empty map created");
	check(!hashmap_iter_valid(it), "iter on empty map is invalid");

	const void *k;
	void *v;
	check(!hashmap_iter_next(it, &k, &v), "iter_next returns false on empty");

	hashmap_iter_free(it);
	hashmap_free(m);
}

static void test_null_args(void) {
	printf("test_null_args\n");

	check(hashmap_size(NULL) == 0, "size(NULL) is 0");
	check(hashmap_is_empty(NULL), "is_empty(NULL) is true");
	check(hashmap_capacity(NULL) == 0, "capacity(NULL) is 0");
	check(!hashmap_contains(NULL, NULL), "contains(NULL,NULL) is false");
	check(hashmap_at(NULL, NULL) == NULL, "at(NULL,NULL) is NULL");
	check(hashmap_insert(NULL, NULL, NULL) == NULL, "insert(NULL,...) is NULL");
	check(hashmap_set(NULL, NULL, NULL) == NULL, "set(NULL,...) is NULL");
	check(!hashmap_remove(NULL, NULL), "remove(NULL,...) is false");
	check(!hashmap_reserve(NULL, 100), "reserve(NULL,...) is false");
	check(!hashmap_defragment(NULL), "defragment(NULL) is false");
	hashmap_free(NULL);

	check(hashmap_iter_new(NULL) == NULL, "iter_new(NULL) is NULL");
	check(!hashmap_iter_valid(NULL), "iter_valid(NULL) is false");
	hashmap_iter_free(NULL);
}

/* ======= Runner ======= */

void run_hashmap_tests(void) {
	tests_passed = 0;
	tests_failed = 0;

	test_new_free();
	test_insert_contains_at();
	test_set();
	test_remove();
	test_null_value();
	test_many_inserts_and_resize();
	test_remove_reinsert();
	test_hash_collision();
	test_reserve();
	test_defragment();
	test_iterator();
	test_iter_set();
	test_iter_empty_map();
	test_null_args();

	printf("\nHashmap tests: %d passed, %d failed\n", tests_passed, tests_failed);
}
