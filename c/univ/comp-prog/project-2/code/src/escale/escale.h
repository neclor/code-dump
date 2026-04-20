/**
 *
 *
*/

#ifndef ESCALE_H
#define ESCALE_H

#include "bool.h"

/* ======= Structures ======= */

typedef struct Escale Escale;

/* ======= Public Function Prototypes ======= */

Escale *escale_create(const char *name, double x, double y);

const char *escale_get_name(const Escale *escale);

double escale_get_x(const Escale *escale);

double escale_get_y(const Escale *escale);

double escale_get_best_time(const Escale *escale);

void escale_set_best_time(Escale *escale, double time);

double escale_distance(const Escale *e1, const Escale *e2);

bool escale_equal(const Escale *e1, const Escale *e2);

#endif // ESCALE_H
