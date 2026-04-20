/**
 *
 *
*/

#include <assert.h>
#include <stdlib.h>

#include "bool.h"
#include "escale.h"

#include "course.h"

/* ======= Constants ======= */

#define INITIAL_SIZE 2
#define REALLOCATION_FACTOR 2

/* ======= Structures ======= */

struct Course {
   size_t escales_count;
   size_t escales_size;
   Escale **escales;
};

/* ======= Private Function Prototypes ======= */

static void course_realloc(Course *course);

/* ======= Public Functions ======= */

Course *course_create(Escale *e1, Escale *e2) {
   assert(
      e1 != NULL &&
      e2 != NULL &&
      escale_get_best_time(e1) == 0 &&
      !escale_equal(e1, e2)
   );

   Escale **escales = malloc(INITIAL_SIZE * sizeof(Escale *));
   if (escales == NULL)
      return NULL;

   Course *course = malloc(sizeof(Course));
   if (course == NULL) {
      free(escales);
      return NULL;
   }

   course->escales_count = 0;
   course->escales_size = INITIAL_SIZE;
   course->escales = escales;

   course_append(course, e1);
   course_append(course, e2);

   return course;
}

bool course_is_circuit(const Course *course) {
   assert(course != NULL);

   if (course->escales_count < 2)
      return false;

   return escale_equal(
      course->escales[0],
      course->escales[course->escales_count - 1]
   );
}

size_t course_get_escales_count(const Course *course) {
   assert(course != NULL);

   return course->escales_count;
}

size_t course_get_stages_count(const Course *course) {
   assert(course != NULL);

   return course->escales_count < 2 ? 0 : course->escales_count - 1;
}

double course_total_time(const Course *course) {
   assert(course != NULL);

   double total_time = 0;

   for (size_t i = 0; i < course->escales_count; i++)
      total_time += escale_get_best_time(course->escales[i]);

   return total_time;
}

double course_best_time_at(const Course *course, size_t index) {
   assert(course != NULL && index < course->escales_count);

   return escale_get_best_time(course->escales[index]);
}

Course *course_append(Course *course, Escale *escale) {
   assert(course != NULL && escale != NULL);

   if (course->escales_count == course->escales_size)
      course_realloc(course);

   course->escales[course->escales_count] = escale;
   course->escales_count++;

   return course;
}

Course *course_pop(Course *course) {
   assert(course != NULL && course->escales_count > 0);

   free(course->escales[course->escales_count - 1]);
   course->escales_count--;
}

void course_free(Course *course) {
   if (course == NULL)
      return;

   if (course->escales != NULL)
      for (size_t i = 0; i < course->escales_count; i++)
         free(course->escales[i]);

   free(course->escales);
   free(course);
}

/* ======= Private Functions ======= */

static void course_realloc(Course *course) {
   assert(course != NULL);

   course->escales_size *= REALLOCATION_FACTOR;
   course->escales = realloc(
      course->escales,
      course->escales_size * sizeof(Escale *)
   );
}
