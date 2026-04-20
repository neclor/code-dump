/**
 *
 *
*/

#include <assert.h>
#include <stdlib.h>

#include "bool.h"
#include "escale.h"

#include "course.h"

/* ======= Structures ======= */

struct Course {
   Escale *escale;
   Course *next;
};

/* ======= Private Function Prototypes ======= */

static const Course *course_last(const Course *course);

/* ======= Public Functions ======= */

Course *course_create(Escale *e1, Escale *e2) {
   assert(
      e1 != NULL &&
      e2 != NULL &&
      escale_get_best_time(e1) == 0 &&
      !escale_equal(e1, e2)
   );

   return course_append(course_append(NULL, e1), e2);
}

bool course_is_circuit(const Course *course) {
   if (course_get_escales_count(course) < 2)
      return false;

   return escale_equal(
      course->escale,
      course_last(course)->escale
   );
}

size_t course_get_escales_count(const Course *course) {
   if (course == NULL)
      return 0;

   return course_get_escales_count(course->next) + 1;
}

size_t course_get_stages_count(const Course *course) {
   if (course == NULL || course->next == NULL)
      return 0;

   return course_get_stages_count(course->next) + 1;
}

double course_total_time(const Course *course) {
   if (course == NULL)
      return 0;

   return (
      course_total_time(course->next) + escale_get_best_time(course->escale)
   );
}

double course_best_time_at(const Course *course, size_t index) {
   assert(course != NULL && index < course_get_escales_count(course));

   if (index == 0)
      return escale_get_best_time(course->escale);

   return course_best_time_at(course->next, index - 1);
}

Course *course_append(Course *course, Escale *escale) {
   assert(escale != NULL);

   if (course == NULL) {
      course = malloc(sizeof(Course));
      if (course == NULL)
         return NULL;
      course->escale = escale;
      course->next = NULL;
      return course;
   }

   course->next = course_append(course->next, escale);

   return course;
}

Course *course_pop(Course *course) {
   assert(course != NULL);

   if (course->next == NULL) {
      free(course->escale);
      free(course);
      return NULL;
   }

   course->next = course_pop(course->next);

   return course;
}

void course_free(Course *course) {
   if (course == NULL)
      return;

   course_free(course->next);
   free(course->escale);
   free(course);
}

/* ======= Private Functions ======= */

static const Course *course_last(const Course *course) {
   assert(course != NULL);

   if (course->next == NULL)
      return course;

   return course_last(course->next);
}
