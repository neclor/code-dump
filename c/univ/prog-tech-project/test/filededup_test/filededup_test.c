#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "filededup.h"

#include "filededup_test.h"

#define DATA_DIR "test/filededup_test/data/"

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

/* ======= Helpers ======= */

static bool dump_contains_path(char **paths, int length, const char *path) {
	for (int i = 0; i < length; i++) {
		if (paths[i] && strcmp(paths[i], path) == 0) return true;
	}
	return false;
}

static int count_nulls(char **paths, int length) {
	int count = 0;
	for (int i = 0; i < length; i++) {
		if (!paths[i]) count++;
	}
	return count;
}

/* ======= Tests ======= */

static void test_init_free(void) {
	printf("test_init_free\n");

	FILEDEDUP fd = FDInit();
	check(fd != NULL, "FDInit returns non-NULL");
	FDFree(fd);

	FDFree(NULL);
	check(true, "FDFree(NULL) does not crash");
}

static void test_check_null_args(void) {
	printf("test_check_null_args\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(NULL, "path") == 0, "FDCheck(NULL, path) returns error");
	check(FDCheck(fd, NULL) == 0, "FDCheck(fd, NULL) returns error");
	check(FDCheck(NULL, NULL) == 0, "FDCheck(NULL, NULL) returns error");

	FDFree(fd);
}

static void test_check_nonexistent_file(void) {
	printf("test_check_nonexistent_file\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, "nonexistent_file_12345.txt") == 0, "FDCheck nonexistent file returns error");

	FDFree(fd);
}

static void test_dump_null_args(void) {
	printf("test_dump_null_args\n");

	FILEDEDUP fd = FDInit();
	int length = -1;

	check(FDDump(NULL, &length) == NULL, "FDDump(NULL, &length) returns NULL");
	check(FDDump(fd, NULL) == NULL, "FDDump(fd, NULL) returns NULL");
	check(FDDump(NULL, NULL) == NULL, "FDDump(NULL, NULL) returns NULL");

	FDFree(fd);
}

static void test_dump_empty(void) {
	printf("test_dump_empty\n");

	FILEDEDUP fd = FDInit();
	int length = -1;

	char **paths = FDDump(fd, &length);
	check(length == 0, "dump empty has length 0");
	free(paths);

	FDFree(fd);
}

static void test_single_file(void) {
	printf("test_single_file\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "c1.txt") == 1, "check single file succeeds");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(length == 0, "dump of single file has length 0");
	free(paths);

	FDFree(fd);
}

static void test_three_different_sizes(void) {
	printf("test_three_different_sizes\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "a1.txt") == 1, "check a1 succeeds");
	check(FDCheck(fd, DATA_DIR "c1.txt") == 1, "check c1 succeeds");
	check(FDCheck(fd, DATA_DIR "d1.txt") == 1, "check d1 succeeds");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(length == 0, "no duplicates among different sizes");
	free(paths);

	FDFree(fd);
}

static void test_duplicate_files_same_content(void) {
	printf("test_duplicate_files_same_content\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "a1.txt") == 1, "check a1 succeeds");
	check(FDCheck(fd, DATA_DIR "a2.txt") == 1, "check a2 succeeds");
	check(FDCheck(fd, DATA_DIR "a3.txt") == 1, "check a3 succeeds");
	check(FDCheck(fd, DATA_DIR "d1.txt") == 1, "check d1 succeeds");
	check(FDCheck(fd, DATA_DIR "d2.txt") == 1, "check d2 succeeds");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(paths != NULL, "dump non-NULL");
	check(dump_contains_path(paths, length, DATA_DIR "a1.txt"), "dump contains a1");
	check(dump_contains_path(paths, length, DATA_DIR "a2.txt"), "dump contains a2");
	check(dump_contains_path(paths, length, DATA_DIR "a3.txt"), "dump contains a3");
	check(dump_contains_path(paths, length, DATA_DIR "d1.txt"), "dump contains d1");
	check(dump_contains_path(paths, length, DATA_DIR "d2.txt"), "dump contains d2");
	check(count_nulls(paths, length) == 2, "two groups: a-duplicates and d-duplicates");
	free(paths);

	FDFree(fd);
}

static void test_size_collision_different_content(void) {
	printf("test_size_collision_different_content\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "b1.txt") == 1, "check b1 succeeds");
	check(FDCheck(fd, DATA_DIR "b2.txt") == 1, "check b2 succeeds");
	check(FDCheck(fd, DATA_DIR "c1.txt") == 1, "check c1 succeeds");
	check(FDCheck(fd, DATA_DIR "d1.txt") == 1, "check d1 succeeds");
	check(FDCheck(fd, DATA_DIR "d2.txt") == 1, "check d2 succeeds");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(paths != NULL, "dump non-NULL");
	check(dump_contains_path(paths, length, DATA_DIR "b1.txt"), "dump contains b1");
	check(dump_contains_path(paths, length, DATA_DIR "b2.txt"), "dump contains b2");
	check(!dump_contains_path(paths, length, DATA_DIR "c1.txt"), "dump does not contain c1 (unique)");
	check(dump_contains_path(paths, length, DATA_DIR "d1.txt"), "dump contains d1");
	check(dump_contains_path(paths, length, DATA_DIR "d2.txt"), "dump contains d2");

	/* b1/b2 in one group, d1/d2 in another; c1 excluded (unique) */
	int null_count = count_nulls(paths, length);
	check(null_count == 2, "2 duplicate groups (b1/b2 and d1/d2)");
	free(paths);

	FDFree(fd);
}

static void test_same_file_twice(void) {
	printf("test_same_file_twice\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "a1.txt") == 1, "first check a1 succeeds");
	check(FDCheck(fd, DATA_DIR "a1.txt") == 0, "second check same path returns error");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(paths != NULL, "dump non-NULL");
	free(paths);

	FDFree(fd);
}

static void test_all_files(void) {
	printf("test_all_files\n");

	FILEDEDUP fd = FDInit();

	check(FDCheck(fd, DATA_DIR "a1.txt") == 1, "check a1");
	check(FDCheck(fd, DATA_DIR "a2.txt") == 1, "check a2");
	check(FDCheck(fd, DATA_DIR "a3.txt") == 1, "check a3");
	check(FDCheck(fd, DATA_DIR "b1.txt") == 1, "check b1");
	check(FDCheck(fd, DATA_DIR "b2.txt") == 1, "check b2");
	check(FDCheck(fd, DATA_DIR "c1.txt") == 1, "check c1");
	check(FDCheck(fd, DATA_DIR "d1.txt") == 1, "check d1");
	check(FDCheck(fd, DATA_DIR "d2.txt") == 1, "check d2");

	int length = 0;
	char **paths = FDDump(fd, &length);
	check(paths != NULL, "dump non-NULL");
	check(length > 0, "dump length > 0");

	check(dump_contains_path(paths, length, DATA_DIR "a1.txt"), "all: contains a1");
	check(dump_contains_path(paths, length, DATA_DIR "a2.txt"), "all: contains a2");
	check(dump_contains_path(paths, length, DATA_DIR "a3.txt"), "all: contains a3");
	check(dump_contains_path(paths, length, DATA_DIR "b1.txt"), "all: contains b1");
	check(dump_contains_path(paths, length, DATA_DIR "b2.txt"), "all: contains b2");
	check(!dump_contains_path(paths, length, DATA_DIR "c1.txt"), "all: c1 excluded (unique)");
	check(dump_contains_path(paths, length, DATA_DIR "d1.txt"), "all: contains d1");
	check(dump_contains_path(paths, length, DATA_DIR "d2.txt"), "all: contains d2");
	check(count_nulls(paths, length) == 3, "all: 3 duplicate groups (a, b, d)");

	printf("\n");
	dump_print(paths, length);

	free(paths);
	FDFree(fd);
}

/* ======= Runner ======= */

void run_filededup_tests(void) {
	tests_passed = 0;
	tests_failed = 0;

	test_init_free();
	test_check_null_args();
	test_check_nonexistent_file();
	test_dump_null_args();
	test_dump_empty();
	test_single_file();
	test_three_different_sizes();
	test_duplicate_files_same_content();
	test_size_collision_different_content();
	test_same_file_twice();
	test_all_files();

	printf("\nFilededup tests: %d passed, %d failed\n", tests_passed, tests_failed);
}

void dump_print(char **paths, int length) {
	int group = 1;
	printf("--- Dump (length=%d) ---\n", length);
	printf("Group %d:\n", group);
	for (int i = 0; i < length; i++) {
		if (paths[i]) {
			printf("  \"%s\"\n", paths[i]);
		} else {
			group++;
			if (i < length - 1) printf("Group %d:\n", group);
		}
	}
	printf("------------------------\n");
}
