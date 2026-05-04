#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Terms.h"
#include "Sort.h"
#include "BinarySearch.h"
#include "AutoComplete.h"


struct AC_t {
	TermArray *ta;
};


AC *acCreate(TermArray *ta) {
	AC *ac = malloc(sizeof(AC));

	if (!ac) {
		fprintf(stderr, "acCreate: allocation error.\n");
		exit(1);
	}

	ac->ta = ta;

	return ac;
}

void acFree(AC *ac) {
	free(ac);
}

size_t acComplete(AC *ac, char *query, size_t k, char **results) {
	size_t posinresults = 0;
	for (size_t i = 0; i < ac->ta->length && posinresults < k; i++) {
		char *termtext = ac->ta->array[i].text;
		size_t j = 0;
		while (query[j] && termtext[j] && query[j] == termtext[j]) {
			j++;
		}
		if (query[j] == '\0') {
			results[posinresults++] = termtext;
		}
	}

	return posinresults;
}
