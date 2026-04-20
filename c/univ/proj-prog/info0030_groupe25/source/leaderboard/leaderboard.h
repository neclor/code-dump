/**
 * @file leaderboard.h
 *
 * @brief High scores management module interface
*/

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdlib.h>

/* ======= Constants ======= */

/**
 * @brief Maximum length of player name including null terminator
 */
#define MAX_NAME_LENGTH 30

/**
 * @brief Maximum number of records stored in leaderboard
 */
#define MAX_RECORDS_COUNT 10

/* ======= Structures ======= */

/**
 * @brief Record structure containing player name and score
 */
typedef struct Record Record;

/**
 * @brief Leaderboard structure containing sorted records
 */
typedef struct Leaderboard Leaderboard;

/* ======= Public Function Prototypes ======= */

/**
 * @brief Creates a new record with given name and score
 *
 * @param name Player name
 * @param score Player score
 *
 * @return Pointer to new Record, NULL if creation failed
 */
Record *create_record(const char *name, unsigned int score);

/**
 * @brief Gets player name from record
 *
 * @param record Pointer to Record structure
 *
 * @return Player name, NULL if record is invalid
 */
const char *record_get_name(const Record *record);

/**
 * @brief Gets score from record
 *
 * @param record Pointer to Record structure
 *
 * @return Player score, 0 if record is invalid
 */
unsigned int record_get_score(const Record *record);

/**
 * @brief Creates new empty leaderboard
 *
 * @return Pointer to new Leaderboard, NULL if creation failed
 */
Leaderboard *create_leaderboard(const char *filename);

/**
 * @brief Saves leaderboard to file
 *
 * @param leaderboard Pointer to Leaderboard structure
 *
 * @return 0 on success, negative value on error
 */
int save_leaderboard(const Leaderboard *leaderboard);

/**
 * @brief Gets number of records in leaderboard
 *
 * @param leaderboard Pointer to Leaderboard structure
 *
 * @return Number of records
 */
size_t get_records_count(const Leaderboard *leaderboard);

/**
 * @brief Gets array of records from leaderboard
 *
 * @param leaderboard Pointer to Leaderboard structure
 *
 * @return Array of Record pointers, NULL if leaderboard is invalid
 */
Record **get_records(const Leaderboard *leaderboard);

/**
 * @brief Adds new record to leaderboard
 *
 * @param leaderboard Pointer to Leaderboard structure
 * @param new_record Record to add
 *
 * @return 0 on success, negative value on error
 */
int add_record(Leaderboard *leaderboard, Record *new_record);

/**
 * @brief Frees memory used by record
 *
 * @param record Pointer to Record structure
 */
void free_record(Record *record);

/**
 * @brief Frees memory used by leaderboard and all its records
 *
 * @param leaderboard Pointer to Leaderboard structure
 */
void free_leaderboard(Leaderboard *leaderboard);

#endif // LEADERBOARD_H
