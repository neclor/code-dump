#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "filededup.h"

#include "filededup_test.h"

typedef struct {
	double wall;
	double cpu;
} BenchResult;

static BenchResult bench_same_size(const char *prefix, int n, size_t size) {
	BenchResult fail = {-1.0, -1.0};
	char (*paths)[256] = malloc((size_t)n * sizeof(*paths));
	if (!paths) return fail;

	for (int i = 0; i < n; i++) {
		snprintf(paths[i], 256, "%s%s_%zu_%d.bin", DIR, prefix, size, i);
	}
	double total_wall = 0.0;
	double total_cpu = 0.0;
	for (int r = 0; r < RUNS; r++) {
		FILEDEDUP fd = FDInit();
		if (!fd) { free(paths); return fail; }

		struct timespec start, end;
		clock_t cpu_start = clock();
		timespec_get(&start, TIME_UTC);
		for (int i = 0; i < n; i++)
			FDCheck(fd, paths[i]);
		timespec_get(&end, TIME_UTC);
		clock_t cpu_end = clock();

		total_wall += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
		total_cpu += (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;
		FDFree(fd);
	}

	free(paths);
	return (BenchResult){total_wall / RUNS, total_cpu / RUNS};
}

static BenchResult bench_different_sizes(int n, size_t base_size) {
	BenchResult fail = {-1.0, -1.0};
	char (*paths)[256] = malloc((size_t)n * sizeof(*paths));
	if (!paths) return fail;

	for (int i = 0; i < n; i++) {
		size_t file_size = base_size + (size_t)(i * 100);
		snprintf(paths[i], 256, "%sdiff_%zu_%d.bin", DIR, file_size, i);
	}

	double total_wall = 0.0;
	double total_cpu = 0.0;
	for (int r = 0; r < RUNS; r++) {
		FILEDEDUP fd = FDInit();
		if (!fd) { free(paths); return fail; }

		struct timespec start, end;
		clock_t cpu_start = clock();
		timespec_get(&start, TIME_UTC);
		for (int i = 0; i < n; i++)
			FDCheck(fd, paths[i]);
		timespec_get(&end, TIME_UTC);
		clock_t cpu_end = clock();

		total_wall += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
		total_cpu += (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;
		FDFree(fd);
	}

	free(paths);
	return (BenchResult){total_wall / RUNS, total_cpu / RUNS};
}

void run_filededup_benchmark(void) {
	static const int counts[] = {100, 200, 400, 600, 800, 1000};
	static const size_t sizes[] = {1048576, 2097152};
	int num_counts = sizeof(counts) / sizeof(counts[0]);
	int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

	for (int s = 0; s < num_sizes; s++) {
		size_t size = sizes[s];
		printf("=== Filededup Benchmark (%d runs averaged, %zu bytes) ===\n\n", RUNS, size);

		printf("%-30s", "N");
		for (int c = 0; c < num_counts; c++)
			printf("%20d", counts[c]);
		printf("\n");

		printf("%-30s", "");
		for (int c = 0; c < num_counts; c++)
			printf("      wall       cpu");
		printf("\n");

		printf("%-30s", "Different sizes");
		for (int c = 0; c < num_counts; c++) {
			generate_different_sizes(counts[c], size);
			BenchResult r = bench_different_sizes(counts[c], size);
			printf("  %7.3fs  %7.3fs", r.wall, r.cpu);
		}
		printf("\n");

		printf("%-30s", "Same size, unique content");
		for (int c = 0; c < num_counts; c++) {
			generate_unique_same_size(counts[c], size);
			BenchResult r = bench_same_size("uniq", counts[c], size);
			printf("  %7.3fs  %7.3fs", r.wall, r.cpu);
		}
		printf("\n");

		printf("%-30s", "Same size, identical content");
		for (int c = 0; c < num_counts; c++) {
			generate_identical(counts[c], size);
			BenchResult r = bench_same_size("dup", counts[c], size);
			printf("  %7.3fs  %7.3fs", r.wall, r.cpu);
		}
		printf("\n\n");
	}

	printf("=== Benchmark Done ===\n");
}
