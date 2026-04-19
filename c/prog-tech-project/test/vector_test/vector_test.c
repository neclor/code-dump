#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"

#include "vector_test.h"

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

	Vector *v = vector_new(sizeof(int));
	check(v != NULL, "vector_new returns non-NULL");
	check(vector_size(v) == 0, "new vector size is 0");
	check(vector_is_empty(v), "new vector is empty");
	check(vector_type_size(v) == sizeof(int), "type_size is sizeof(int)");
	check(vector_capacity(v) == 0, "new vector capacity is 0");
	vector_free(v);
}

static void test_push_back_and_at(void) {
	printf("test_push_back_and_at\n");

	Vector *v = vector_new(sizeof(int));
	int val = 42;

	void *ret = vector_push_back(v, &val);
	check(ret != NULL, "push_back returns non-NULL");
	check(*(int *)ret == 42, "push_back returns correct value");
	check(vector_size(v) == 1, "size is 1 after push_back");
	check(!vector_is_empty(v), "vector not empty after push_back");

	void *at = vector_at(v, 0);
	check(at != NULL, "at(0) returns non-NULL");
	check(*(int *)at == 42, "at(0) returns correct value");

	check(vector_at(v, 1) == NULL, "at(1) out of bounds returns NULL");
	check(vector_at(v, 100) == NULL, "at(100) out of bounds returns NULL");

	vector_free(v);
}

static void test_front_back(void) {
	printf("test_front_back\n");

	Vector *v = vector_new(sizeof(int));

	check(vector_front(v) == NULL, "front on empty returns NULL");
	check(vector_back(v) == NULL, "back on empty returns NULL");

	int a = 10, b = 20, c = 30;
	vector_push_back(v, &a);
	vector_push_back(v, &b);
	vector_push_back(v, &c);

	check(*(int *)vector_front(v) == 10, "front is 10");
	check(*(int *)vector_back(v) == 30, "back is 30");

	vector_free(v);
}

static void test_set(void) {
	printf("test_set\n");

	Vector *v = vector_new(sizeof(int));
	int a = 1, b = 2, c = 99;
	vector_push_back(v, &a);
	vector_push_back(v, &b);

	void *ret = vector_set(v, 0, &c);
	check(ret != NULL && *(int *)ret == 99, "set returns new value");
	check(*(int *)vector_at(v, 0) == 99, "at reflects set value");
	check(*(int *)vector_at(v, 1) == 2, "other element unchanged");

	check(vector_set(v, 5, &c) == NULL, "set out of bounds returns NULL");

	ret = vector_set(v, 1, NULL);
	check(ret != NULL && *(int *)ret == 0, "set NULL zeroes element");

	vector_free(v);
}

static void test_insert(void) {
	printf("test_insert\n");

	Vector *v = vector_new(sizeof(int));
	int a = 10, b = 20, c = 15;

	vector_push_back(v, &a);
	vector_push_back(v, &b);

	void *ret = vector_insert(v, 1, &c);
	check(ret != NULL && *(int *)ret == 15, "insert returns value");
	check(vector_size(v) == 3, "size is 3 after insert");
	check(*(int *)vector_at(v, 0) == 10, "element 0 correct");
	check(*(int *)vector_at(v, 1) == 15, "inserted element correct");
	check(*(int *)vector_at(v, 2) == 20, "shifted element correct");

	int d = 5;
	vector_insert(v, 0, &d);
	check(*(int *)vector_front(v) == 5, "insert at 0 becomes front");
	check(vector_size(v) == 4, "size is 4");

	check(vector_insert(v, 10, &d) == NULL, "insert beyond size returns NULL");

	vector_free(v);
}

static void test_remove_at(void) {
	printf("test_remove_at\n");

	Vector *v = vector_new(sizeof(int));
	for (int i = 0; i < 5; i++) {
		vector_push_back(v, &i);
	}

	vector_remove_at(v, 2);
	check(vector_size(v) == 4, "size 4 after remove_at(2)");
	check(*(int *)vector_at(v, 0) == 0, "element 0 intact");
	check(*(int *)vector_at(v, 1) == 1, "element 1 intact");
	check(*(int *)vector_at(v, 2) == 3, "element 2 shifted from 3");
	check(*(int *)vector_at(v, 3) == 4, "element 3 shifted from 4");

	vector_remove_at(v, 0);
	check(vector_size(v) == 3, "size 3 after remove_at(0)");
	check(*(int *)vector_front(v) == 1, "front is 1 after remove first");

	vector_remove_at(v, 10);
	check(vector_size(v) == 3, "remove_at out of bounds is no-op");

	vector_free(v);
}

static void test_pop_back(void) {
	printf("test_pop_back\n");

	Vector *v = vector_new(sizeof(int));
	int a = 1, b = 2;
	vector_push_back(v, &a);
	vector_push_back(v, &b);

	vector_pop_back(v);
	check(vector_size(v) == 1, "size 1 after pop_back");
	check(*(int *)vector_back(v) == 1, "back is 1 after pop");

	vector_pop_back(v);
	check(vector_size(v) == 0, "size 0 after second pop");
	check(vector_is_empty(v), "empty after popping all");

	vector_pop_back(v);
	check(vector_size(v) == 0, "pop_back on empty is no-op");

	vector_free(v);
}

static void test_reserve(void) {
	printf("test_reserve\n");

	Vector *v = vector_new(sizeof(int));
	check(vector_reserve(v, 100), "reserve 100 succeeds");
	check(vector_capacity(v) >= 100, "capacity >= 100");
	check(vector_size(v) == 0, "size still 0 after reserve");

	size_t cap = vector_capacity(v);
	check(!vector_reserve(v, 50), "reserve smaller fails");
	check(vector_capacity(v) == cap, "capacity unchanged after failed reserve");

	check(!vector_reserve(v, cap), "reserve same capacity fails");

	vector_free(v);
}

static void test_push_back_null_value(void) {
	printf("test_push_back_null_value\n");

	Vector *v = vector_new(sizeof(int));

	void *ret = vector_push_back(v, NULL);
	check(ret != NULL, "push_back NULL returns non-NULL");
	check(*(int *)ret == 0, "push_back NULL zeroes element");
	check(vector_size(v) == 1, "size is 1");

	vector_free(v);
}

static void test_many_push_backs(void) {
	printf("test_many_push_backs\n");

	Vector *v = vector_new(sizeof(int));
	int n = 500;

	for (int i = 0; i < n; i++) {
		vector_push_back(v, &i);
	}
	check(vector_size(v) == (size_t)n, "size correct after 500 pushes");
	check(vector_capacity(v) >= (size_t)n, "capacity >= 500");

	for (int i = 0; i < n; i++) {
		check(*(int *)vector_at(v, i) == i, "value correct after growth");
	}

	vector_free(v);
}

static void test_large_type(void) {
	printf("test_large_type\n");

	typedef struct { int x; int y; double z; } Point;
	Vector *v = vector_new(sizeof(Point));
	check(vector_type_size(v) == sizeof(Point), "type_size matches struct");

	Point p1 = {1, 2, 3.14};
	Point p2 = {4, 5, 6.28};
	vector_push_back(v, &p1);
	vector_push_back(v, &p2);

	Point *out = vector_at(v, 0);
	check(out->x == 1 && out->y == 2 && out->z == 3.14, "struct element 0 correct");
	out = vector_at(v, 1);
	check(out->x == 4 && out->y == 5 && out->z == 6.28, "struct element 1 correct");

	vector_free(v);
}

static void test_iterator(void) {
	printf("test_iterator\n");

	Vector *v = vector_new(sizeof(int));
	for (int i = 0; i < 5; i++) {
		vector_push_back(v, &i);
	}

	VectorIter *it = vector_iter_new(v);
	check(it != NULL, "iter created");
	check(vector_iter_valid(it), "iter valid at start");
	check(vector_iter_index(it) == 0, "iter index starts at 0");

	int count = 0;
	while (vector_iter_valid(it)) {
		void *val = vector_iter_next(it);
		check(*(int *)val == count, "iter value matches index");
		count++;
	}
	check(count == 5, "iter visited all 5 elements");
	check(!vector_iter_valid(it), "iter invalid after exhaustion");
	check(vector_iter_next(it) == NULL, "iter_next returns NULL after end");

	vector_iter_free(it);
	vector_free(v);
}

static void test_iter_at_and_set(void) {
	printf("test_iter_at_and_set\n");

	Vector *v = vector_new(sizeof(int));
	int a = 10, b = 20;
	vector_push_back(v, &a);
	vector_push_back(v, &b);

	VectorIter *it = vector_iter_new(v);

	void *at = vector_iter_at(it);
	check(at != NULL && *(int *)at == 10, "iter_at returns first element");

	int new_val = 999;
	void *ret = vector_iter_set(it, &new_val);
	check(ret != NULL && *(int *)ret == 999, "iter_set returns new value");
	check(*(int *)vector_at(v, 0) == 999, "vector reflects iter_set");

	vector_iter_next(it);
	at = vector_iter_at(it);
	check(at != NULL && *(int *)at == 20, "iter_at on second element");

	vector_iter_free(it);
	vector_free(v);
}

static void test_iter_empty_vector(void) {
	printf("test_iter_empty_vector\n");

	Vector *v = vector_new(sizeof(int));
	VectorIter *it = vector_iter_new(v);
	check(it != NULL, "iter on empty vector created");
	check(!vector_iter_valid(it), "iter on empty vector is invalid");
	check(vector_iter_at(it) == NULL, "iter_at on empty returns NULL");
	check(vector_iter_next(it) == NULL, "iter_next on empty returns NULL");
	check(vector_iter_set(it, NULL) == NULL, "iter_set on empty returns NULL");

	vector_iter_free(it);
	vector_free(v);
}

static void test_null_args(void) {
	printf("test_null_args\n");

	check(vector_size(NULL) == 0, "size(NULL) is 0");
	check(vector_capacity(NULL) == 0, "capacity(NULL) is 0");
	check(vector_type_size(NULL) == 0, "type_size(NULL) is 0");
	check(vector_is_empty(NULL), "is_empty(NULL) is true");
	check(vector_at(NULL, 0) == NULL, "at(NULL,0) is NULL");
	check(vector_front(NULL) == NULL, "front(NULL) is NULL");
	check(vector_back(NULL) == NULL, "back(NULL) is NULL");
	check(vector_set(NULL, 0, NULL) == NULL, "set(NULL,...) is NULL");
	check(vector_insert(NULL, 0, NULL) == NULL, "insert(NULL,...) is NULL");
	check(vector_push_back(NULL, NULL) == NULL, "push_back(NULL,...) is NULL");
	check(!vector_reserve(NULL, 10), "reserve(NULL,...) is false");
	vector_remove_at(NULL, 0);
	vector_pop_back(NULL);
	vector_free(NULL);

	check(vector_iter_new(NULL) == NULL, "iter_new(NULL) is NULL");
	check(!vector_iter_valid(NULL), "iter_valid(NULL) is false");
	check(vector_iter_index(NULL) == 0, "iter_index(NULL) is 0");
	vector_iter_free(NULL);
}

/* ======= Runner ======= */

void run_vector_tests(void) {
	tests_passed = 0;
	tests_failed = 0;

	test_new_free();
	test_push_back_and_at();
	test_front_back();
	test_set();
	test_insert();
	test_remove_at();
	test_pop_back();
	test_reserve();
	test_push_back_null_value();
	test_many_push_backs();
	test_large_type();
	test_iterator();
	test_iter_at_and_set();
	test_iter_empty_vector();
	test_null_args();

	printf("\nVector tests: %d passed, %d failed\n", tests_passed, tests_failed);
}
