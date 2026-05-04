#ifndef TERMS_H_
#define TERMS_H_


#include <stdlib.h>


typedef struct Term_t {
	char *text;
	long weight;
} Term;

typedef struct TermArray_t {
	Term *array;
	size_t length;
} TermArray;

/* ------------------------------------------------------------------------- *
 * Load a set of terms and their weights from a file. The resulting array
 * needs to be freed by termsFree.
 *
 * PARAMETERS
 * filename           A txt file containing terms and their weights. The terms
 *                    are assumed to be ordered in decreasing order of weights.
 *
 * RETURN
 * ta                 An array of terms (TermArray)
 * ------------------------------------------------------------------------- */
TermArray *termsLoadFile(char *filename);

/* ------------------------------------------------------------------------- *
 * Free an array of terms.
 *
 * PARAMETERS
 * ta                  An array of terms
 * ------------------------------------------------------------------------- */
void termsFree(TermArray *ta);

/* ------------------------------------------------------------------------- *
 * Print the first k terms from an array of terms in the fp file (stdout to
 * print in the terminal). Can be used for debugging.
 *
 * PARAMETERS
 * ta                 An array of terms
 * k                  The number of terms to be printed
 * fp                 The file in which the terms will be printed
 * ------------------------------------------------------------------------- */

void termsPrint(TermArray *ta, size_t k, FILE *fp);


#endif // TERMS_H_
