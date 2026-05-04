/* Interface for an auto-completion structure (type AC)
 */
#ifndef AC_H_
#define AC_H_


#include <stdlib.h>
#include <stdio.h>

#include "Terms.h"


typedef struct AC_t AC;

/* ------------------------------------------------------------------------- *
 * Create a structure for performing auto-completion. The function can reorder
 * the array of terms. The returned structure should be freed by acFree. The
 * latter should not free the term array.
 *
 * PARAMETERS
 * ta                 An array of terms with their weights (see Terms.h)
 *
 * RETURN
 * ac                 An auto-completion structure
 * ------------------------------------------------------------------------- */
AC *acCreate(TermArray *ta);

/* ------------------------------------------------------------------------- *
 * Free the auto-completion structure.
 *
 * PARAMETERS
 * ac                 An auto-completion structure created by acCreate
 * ------------------------------------------------------------------------- */
void acFree(AC *ac);

/* ------------------------------------------------------------------------- *
 * Performs an auto-completion using the query.
 *
 * PARAMETERS
 * ac                 An auto-completion structure (returned by acCreate)
 * query              A string containing the query
 * k                  The number of terms to return
 * results            An array of size k of char * that will contain the
 *                    returned terms
 *
 * RETURN
 * size               the number of found terms (between 0 and k)
 * ------------------------------------------------------------------------- */
size_t acComplete(AC *ac, char *query, size_t k, char **results);


#endif // AC_H_
