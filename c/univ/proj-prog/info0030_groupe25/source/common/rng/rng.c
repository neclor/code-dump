/**
 * @file rng.c
 *
 * @brief Random number generation implementation
*/

#include <stdlib.h>
#include <time.h>

#include "rng.h"

/* ======= Public Functions ======= */

void randomize(void) {
   srand(time(NULL));
}

int randi(void) {
   return rand();
}

int randi_range(int from, int to) {
   if (from > to) {
      int tmp = from;
      from = to;
      to = tmp;
   }
   return from + randi() % (to - from + 1);
}
