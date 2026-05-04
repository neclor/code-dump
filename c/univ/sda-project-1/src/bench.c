#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Terms.h"
#include "Sort.h"
#include "AutoComplete.h"

#define TRIALS 100
#define MAX_L 10
#define K_RESULTS 20


static void termArraySwap(void *array, size_t i, size_t j);
static int termArrayCompareByText(const void *array, size_t i, size_t j);
static int termArrayCompareByWeight(const void *array, size_t i, size_t j);


static void benchmark_sorting(char *filename);
static void benchmark_completion(char *filename);


int main() {
    srand(time(NULL));

	char *files[] = {
        "../Data/actors_25k.txt",
        "../Data/actors_250k.txt",
        "../Data/actors_2500k.txt"
    };

    for (int i = 0; i < 3; i++) {
        //benchmark_sorting(files[i]);
        benchmark_completion(files[i]);
    }

    return 0;
}


static void benchmark_sorting(char *filename) {
    TermArray *ta = termsLoadFile(filename);
    if (!ta) return;

    clock_t start, end;

    // Тест 1: Лексикографическая сортировка
    start = clock();
    sort(ta, ta->length, termArrayCompareByText, termArraySwap);
    end = clock();
    double time_lex = (double)(end - start) / CLOCKS_PER_SEC;


    termsFree(ta);
    ta = termsLoadFile(filename);

    // Тест 2: Сортировка по весу
    start = clock();
    sort(ta, ta->length, termArrayCompareByWeight, termArraySwap);
    end = clock();
    double time_weight = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n--- SORT: %s ---\n", filename);
    printf("Lexicographical: %f sec\n", time_lex);
    printf("Weight:          %f sec\n", time_weight);
	printf("\n");

    termsFree(ta);
}

static void benchmark_completion(char *filename) {
    TermArray *ta = termsLoadFile(filename);
    if (!ta) return;

    AC *ac = acCreate(ta);
    char **results = malloc(K_RESULTS * sizeof(char *));

    printf("\n--- ПОИСК (AVG TIME OVER %d TRIALS): %s ---\n", TRIALS, filename);
    printf("l (length)\tTime (ms)\n");

    for (int l = 0; l <= MAX_L; l++) {
        clock_t total_ticks = 0;

        for (int t = 0; t < TRIALS; t++) {
            size_t idx = rand() % ta->length;
            char query[MAX_L + 1];

            size_t text_len = strlen(ta->array[idx].text);
            size_t copy_len = (text_len < (size_t)l) ? text_len : (size_t)l;
            strncpy(query, ta->array[idx].text, copy_len);
            query[copy_len] = '\0';

            clock_t start = clock();
            acComplete(ac, query, K_RESULTS, results);
            clock_t end = clock();

            total_ticks += (end - start);
        }

        double avg_ms = ((double)total_ticks / TRIALS) * 1000.0 / CLOCKS_PER_SEC;
        printf("%d\t\t%f\n", l, avg_ms);
    }

    free(results);
    acFree(ac);
    termsFree(ta);
}


static void termArraySwap(void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termArraySwap: array is NULL.\n");
		exit(1);
	}
	TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i || length <= j) {
		fprintf(stderr, "termArraySwap: index out of bounds.\n");
		exit(1);
	}
	if (i == j) return;
	Term *terms = ta->array;

	Term tmp = terms[i];
	terms[i] = terms[j];
	terms[j] = tmp;
}

static int termArrayCompareByText(const void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termArrayCompareByText: array is NULL.\n");
		exit(1);
	}
	const TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i || length <= j) {
		fprintf(stderr, "termArrayCompareByText: index out of bounds.\n");
		exit(1);
	}
	if (i == j) return 0;
	const Term *terms = ta->array;

	char *t1 = terms[i].text;
	char *t2 = terms[j].text;

	if (t1 == NULL && t2 == NULL) return 0;
	if (t1 == NULL) return -1;
	if (t2 == NULL) return 1;
	return strcmp(t1, t2);
}

static int termArrayCompareByWeight(const void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termArrayCompareByWeight: array is NULL.\n");
		exit(1);
	}
	const TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i || length <= j) {
		fprintf(stderr, "termArrayCompareByWeight: index out of bounds.\n");
		exit(1);
	}
	if (i == j) return 0;
	const Term *terms = ta->array;

	long w1 = terms[i].weight;
	long w2 = terms[j].weight;

	if (w1 < w2) return -1;
	if (w1 > w2) return 1;
	return 0;
}
