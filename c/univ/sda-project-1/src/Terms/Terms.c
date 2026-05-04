#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Terms.h"


static char *strDuplicate(const char *s) {
	char *dup = malloc(strlen(s) + 1);

	if (dup != NULL)
	{
		size_t i = 0;
		for (i = 0; s[i]; i++)
			dup[i] = tolower(s[i]);
		dup[i] = '\0';
	}

	return dup;
}

TermArray *termsLoadFile(char *filename) {
	char buffer[2000];
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "termsLoadFile: error opening file %s.\n", filename);
		exit(1);
	}

	if (!fgets(buffer, sizeof(buffer), fp)) {
		fprintf(stderr, "loadTerms: file is corrupted.\n");
		exit(1);
	}

	size_t nbTerms = atol(buffer);

	TermArray *termarray = malloc(sizeof(TermArray));
	if (termarray == NULL) {
		fprintf(stderr, "loadFile: allocation error.\n");
		exit(1);
	}

	termarray->array = malloc(nbTerms * sizeof(Term));
	if (termarray->array == NULL) {
		fprintf(stderr, "loadFile: Allocation error.\n");
		exit(1);
	}
	termarray->length = nbTerms;

	size_t i = 0;
	while (i < nbTerms && fgets(buffer, sizeof(buffer), fp)) {
		int lenstr = strlen(buffer) - 1;
		buffer[lenstr] = '\0'; // replace \n with \0

		char *weightc = strtok(buffer, "\t");
		char *text = strtok(NULL, "\t");

		termarray->array[i].weight = atol(weightc);
		termarray->array[i].text = strDuplicate(text);

		i++;
	}
	return termarray;
}

void termsFree(TermArray *ta) {
	for (size_t i = 0; i < ta->length; i++) {
		free(ta->array[i].text);
	}

	free(ta->array);
	free(ta);
}

void termsPrint(TermArray *ta, size_t k, FILE *fp) {
	for (size_t i = 0; i < k && i < ta->length; i++) {
		fprintf(fp, "(%ld) %s\n", ta->array[i].weight, ta->array[i].text);
	}
}
