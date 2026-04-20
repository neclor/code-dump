#include <assert.h>
#include <stdlib.h>

#include "prefixe_suffixe.h"

/* ======= Private Function Prototypes ======= */

static int pref_equal_suff(int *T, unsigned int N, unsigned int k);

/* ======= Public Functions ======= */

/**
 * Sp 1
 * Checking all prefixes starting from the longest one until we find a match
 * or exhaust all possibilities.
 */
int prefixe_suffixe(int *T, unsigned int N) {
   assert((T != NULL) && (0 < N));
   // {$ 0 < N $}
   unsigned int k = N - 1;
   // {$ k = N - 1 $}
   while (0 < k && !pref_equal_suff(T, N, k)) {
      // {$ 0 < k < N \land \lnot pref\_equal\_suff(T, N, k) $}
      k--;
   }
   // {$ k = 0 \lor pref\_equal\_suff(T, N, k) $}
   return k;
   // {$ T = T_0 \land N = N_0 $}
}

/* ======= Private Functions ======= */

/**
 * Sp 2
 * Comparing the prefix and suffix of the given length, element by element.
 */
static int pref_equal_suff(int *T, unsigned int N, unsigned int k) {
   assert((T != NULL) && (0 < N) && (0 < k && k < N));
   // {$ 0 < k < N $}
   unsigned int i = 0;
   // {$ i = 0 $}
   while (i < k && T[i] == T[N - k + i]) {
      // {$ i < k \land T[i] = T[N - k + i] $}
      i++;
   }
   // {$ i = k \lor T[i] \neq T[N - k + i] $}
   return i == k;
   // {$ T = T_0 \land N = N_0 $}
}
