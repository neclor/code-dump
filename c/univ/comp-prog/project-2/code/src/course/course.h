/**
 *
 *
*/

#ifndef COURSE_H
#define COURSE_H

#include <stdlib.h>

#include "bool.h"
#include "escale.h"

/* ======= Structures ======= */

typedef struct Course Course;

/* ======= Public Function Prototypes ======= */

Course *course_create(Escale *e1, Escale *e2);

bool course_is_circuit(const Course *course);

size_t course_get_escales_count(const Course *course);

size_t course_get_stages_count(const Course *course);

double course_total_time(const Course *course);

double course_best_time_at(const Course *course, size_t index);

Course *course_append(Course *course, Escale *escale);

Course *course_pop(Course *course);

void course_free(Course *course);

#endif // COURSE_H
