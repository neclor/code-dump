/**
 * @file leaderboard.c
 *
 * @brief High scores management module implementation
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leaderboard.h"

/* ======= Structures ======= */

struct Record {
   char name[MAX_NAME_LENGTH + 1];
   unsigned int score;
};

struct Leaderboard {
   size_t records_count;
   Record **records;
   const char *filename;
};

/* ======= Private Function Prototypes ======= */

/**
 * @brief Checks if two records have the same name
 *
 * @param r1 First record
 * @param r2 Second record
 *
 * @return true if names match, false otherwise
 */
static bool record_name_equal(const Record *r1, const Record *r2);

/**
 * @brief Loads leaderboard from file
 *
 * @param leaderboard Pointer to Leaderboard structure
 *
 * @return 0 on success, negative value on error
 */
static int load_leaderboard(Leaderboard *leaderboard);

/* ======= Public Functions ======= */

Record *create_record(const char *name, unsigned int score) {
   if (name == NULL)
      return NULL;

   Record *record = malloc(sizeof(Record));
   if (record == NULL)
      return NULL;

   strncpy(record->name, name, MAX_NAME_LENGTH + 1);
   record->name[MAX_NAME_LENGTH] = '\0';

   record->score = score;

   return record;
}

const char *record_get_name(const Record *record) {
   if (record == NULL)
      return NULL;

   const char *name = record->name;

   return name;
}

unsigned int record_get_score(const Record *record) {
   if (record == NULL)
      return 0;

   return record->score;
}

Leaderboard *create_leaderboard(const char *filename) {
   Leaderboard *leaderboard = malloc(sizeof(Leaderboard));
   if (leaderboard == NULL)
      return NULL;

   leaderboard->records_count = 0;
   leaderboard->records = malloc(MAX_RECORDS_COUNT * sizeof(Record *));

   leaderboard->filename =
      filename != NULL ? strcpy(malloc(strlen(filename) + 1), filename) : NULL;

   for (size_t i = 0; i < MAX_RECORDS_COUNT; i++) {
      leaderboard->records[i] = NULL;
   }

   if (leaderboard->records == NULL) {
      free(leaderboard);
      return NULL;
   }

   load_leaderboard(leaderboard);

   return leaderboard;
}

int save_leaderboard(const Leaderboard *leaderboard) {
   if (leaderboard == NULL || leaderboard->filename == NULL)
      return -1;

   FILE *file = fopen(leaderboard->filename, "w");
   if (file == NULL)
      return -1;

   for (size_t i = 0; i < leaderboard->records_count; i++) {
      Record *record = leaderboard->records[i];

      fprintf(file, "%s\n%u\n", record->name, record->score);
   }

   fclose(file);
   return 0;
}

size_t get_records_count(const Leaderboard *leaderboard) {
   return leaderboard == NULL ? 0 : leaderboard->records_count;
}

Record **get_records(const Leaderboard *leaderboard) {
   return leaderboard == NULL ? NULL :leaderboard->records;
}

int add_record(Leaderboard *leaderboard, Record *new_record) {
   if (leaderboard == NULL || new_record == NULL)
      return -1;

   size_t records_count = leaderboard->records_count;
   Record **records = leaderboard->records;
   if (records == NULL)
      return -1;

   size_t pos = records_count;
   bool is_new_record = true;
   for (size_t i = 0; i < records_count; i++) {
      if (record_name_equal(records[i], new_record)) {
         is_new_record = false;
         if (new_record->score <= records[i]->score) {
            free_record(new_record);
            return 0;
         }
         pos = i;
         free_record(records[i]);
         break;
      }
   }

   if (is_new_record && leaderboard->records_count < MAX_RECORDS_COUNT)
      leaderboard->records_count++;

   for (; pos > 0; pos--) {
      if (new_record->score > records[pos - 1]->score) {
         if (pos >= MAX_RECORDS_COUNT) {
            free_record(records[pos - 1]);
            continue;
         }
         records[pos] = records[pos - 1];
         records[pos - 1] = NULL;
      } else {
         break;
      }
   }

   if (pos < MAX_RECORDS_COUNT) {
      records[pos] = new_record;
   } else {
      free_record(new_record);
   }

   return 0;
}

void free_record(Record *record) {
   free(record);
}

void free_leaderboard(Leaderboard *leaderboard) {
   if (leaderboard == NULL)
      return;

   if (leaderboard->records == NULL) {
      free(leaderboard);
      return;
   }

   for (size_t i = 0; i < leaderboard->records_count; i++)
      free_record(leaderboard->records[i]);

   free(leaderboard->records);
   free(leaderboard);
}

/* ======= Private Functions ======= */

static bool record_name_equal(const Record *r1, const Record *r2) {
   if (r1 == NULL || r2 == NULL)
      return false;

   return strcmp(r1->name, r2->name) == 0;
}

static int load_leaderboard(Leaderboard *leaderboard) {
   if (leaderboard == NULL || leaderboard->filename == NULL)
      return -1;

   FILE *file = fopen(leaderboard->filename, "r");
   if (file == NULL)
      return 0;

   char name[MAX_NAME_LENGTH + 1];
   while (fgets(name, MAX_NAME_LENGTH + 1, file) != NULL) {
      size_t len = strlen(name);
      if (len > 0 && name[len-1] == '\n') {
         name[len-1] = '\0';
      }

      unsigned int score;

      char score_str[32];
      if (fgets(score_str, sizeof(score_str), file) == NULL)
         break;

      if (sscanf(score_str, "%u", &score) != 1)
         continue;

      Record *record = create_record(name, score);
      if (record != NULL)
         add_record(leaderboard, record);
   }

   fclose(file);
   return 0;
}
