#ifndef FILEDEDUP_TEST_H
#define FILEDEDUP_TEST_H

#define DIR "test/filededup_test/generated/"
#define RUNS 10

void run_filededup_tests(void);

void generate_different_sizes(int n, size_t base_size);
void generate_unique_same_size(int n, size_t size);
void generate_identical(int n, size_t size);

void run_filededup_benchmark(void);

void dump_print(char **paths, int length);

#endif // FILEDEDUP_TEST_H
