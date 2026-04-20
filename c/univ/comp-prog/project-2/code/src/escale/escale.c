/**
 *
 *
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "bool.h"

#include "escale.h"

/* ======= Constants =======*/

#define PI 3.14159265358979323846

/* ======= Structures ======= */

struct Escale {
   const char *name;
   double x; // lon
   double y; // lat
   double best_time;
};

/* ======= Private Function Prototypes ======= */

static double haversine_distance(
   double lat1,
   double lon1,
   double lat2,
   double lon2
);

/* ======= Public Functions ======= */

Escale *escale_create(const char *name, double x, double y) {
   assert(
      name != NULL &&
      -180 <= x  && x <= 180 &&
      -90 <= y && y <= 90
   );

   Escale *escale = malloc(sizeof(Escale));
   if (escale == NULL)
      return NULL;

   escale->name = name;
   escale->x = x;
   escale->y = y;
   escale->best_time = 0;

   return escale;
}

const char *escale_get_name(const Escale *escale) {
   assert(escale != NULL);

   return escale->name;
}

double escale_get_x(const Escale *escale) {
   assert(escale != NULL);

   return escale->x;
}

double escale_get_y(const Escale *escale) {
   assert(escale != NULL);

   return escale->y;
}

double escale_get_best_time(const Escale *escale) {
   assert(escale != NULL);

   return escale->best_time;
}

void escale_set_best_time(Escale *escale, double best_time) {
   assert(escale != NULL);

   escale->best_time = best_time;
}

double escale_distance(const Escale *e1, const Escale *e2) {
   assert(e1 != NULL && e2 != NULL);

   return haversine_distance(e1->y, e1->x, e2->y, e2->x);
}

bool escale_equal(const Escale *e1, const Escale *e2) {
   assert(e1 != NULL && e2 != NULL);

   return e1->x == e2->x && e1->y == e2->y;
}

/* ======= Private Functions ======= */

static double haversine_distance(
   double lat1,
   double lon1,
   double lat2,
   double lon2
) {
   // https://en.wikipedia.org/wiki/Haversine_formula#Formulation

   const double radius = 6371.0;

   double phi1 = lat1 * PI / 180;
   double phi2 = lat2 * PI / 180;
   double lam1 = lon1 * PI / 180;
   double lam2 = lon2 * PI / 180;

   double delta_phi = phi2 - phi1;
   double delta_lambda = lam2 - lam1;

   double a = cos(phi1) * cos(phi2) * (1 - cos(delta_lambda));
   double b = (1 - cos(delta_phi) + a) / 2;
   double distance = 2 * radius * asin(sqrt(b));

   return distance;
}
