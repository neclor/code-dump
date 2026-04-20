/**
 *
 *
*/

#include "seatest.h"

#include "course.h"

/* ======= Functions ======= */

static void test_course_append(void) {
   Escale *liege = escale_create("Liege", 5.5796662, 50.6325574);
   Escale *namur = escale_create("Namur", 4.86, 50.47);
   Escale *brussels = escale_create("Brussels", 4.3517, 50.8503);
   Escale *mons = escale_create("Mons", 3.95, 50.45);

   Course* course = course_append(NULL, liege);

   assert_ulong_equal(1, course_get_escales_count(course));

   assert_true(
      course == course_append(
         course_append(course, namur),
         brussels
      )
   );

   assert_ulong_equal(3, course_get_escales_count(course));

   course_append(course_pop(course), mons);

   assert_ulong_equal(3, course_get_escales_count(course));

   course_free(course);
}

static void test_course_total_time(void) {
   assert_double_equal(0.0, course_total_time(NULL), 0.001);

   Escale *liege = escale_create("Liege", 5.5796662, 50.6325574);
   Escale *namur = escale_create("Namur", 4.86, 50.47);
   Escale *brussels = escale_create("Brussels", 4.3517, 50.8503);
   Escale *mons = escale_create("Mons", 3.95, 50.45);

   Course* course = course_create(liege, namur);
   course_append(course, brussels);
   course_append(course, mons);

   assert_double_equal(0.0, course_total_time(course), 0.001);

   escale_set_best_time(namur, 100.0);
   escale_set_best_time(brussels, 100.0);
   escale_set_best_time(mons, 100.0);

   assert_double_equal(300.0, course_total_time(course), 0.001);

   Escale *liege2 = escale_create("Liege", 5.5796662, 50.6325574);
   escale_set_best_time(liege2, 100.0);
   course_append(course, liege2);

   assert_double_equal(400.0, course_total_time(course), 0.001);

   course_pop(course);
   course_pop(course);

   assert_double_equal(200.0, course_total_time(course), 0.001);

   course_free(course);
}

static void test_fixture(void) {
   test_fixture_start();
   run_test(test_course_append);
   run_test(test_course_total_time);
   test_fixture_end();
}

static void all_tests(void) {
   test_fixture();
}

int main() {
   return run_tests(all_tests);
}
