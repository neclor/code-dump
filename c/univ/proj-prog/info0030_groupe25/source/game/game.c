/**
 * @file game.c
 *
 * @brief Implementation of the game logic module
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "point.h"
#include "rng.h"

#include "game.h"

/* ======= Constants ======= */

#define EASY_COLORS_COUNT 5
#define MEDIUM_COLORS_COUNT 7
#define HARD_COLORS_COUNT 7

#define PATH_DIRECTION_COUNT 4

#define MIN_LINE_LENGTH 5
#define MAX_LINE_LENGTH 14

static const int EASY_FIELD_WIDTH = 7;
static const int EASY_FIELD_HEIGHT = 7;
static const int MEDIUM_FIELD_WIDTH = 9;
static const int MEDIUM_FIELD_HEIGHT = 9;
static const int HARD_FIELD_WIDTH = 20;
static const int HARD_FIELD_HEIGHT = 15;

static const Cell EASY_COLORS[EASY_COLORS_COUNT] = {CELL_RED, CELL_ORANGE,
   CELL_YELLOW, CELL_GREEN, CELL_CYAN
};

static const Cell MEDIUM_COLORS[MEDIUM_COLORS_COUNT] = {CELL_RED, CELL_ORANGE,
   CELL_YELLOW, CELL_GREEN, CELL_CYAN, CELL_BLUE, CELL_PURPLE
};

static const Cell HARD_COLORS[HARD_COLORS_COUNT] = {CELL_RED, CELL_ORANGE,
   CELL_YELLOW, CELL_GREEN, CELL_CYAN, CELL_BLUE, CELL_PURPLE
};

static const size_t EASY_NEXT_BALLS_COUNT = 3;
static const size_t MEDIUM_NEXT_BALLS_COUNT = 3;
static const size_t HARD_NEXT_BALLS_COUNT = 7;

static const Point PATH_DIRECTIONS[PATH_DIRECTION_COUNT] = {POINT_UP,
   POINT_DOWN, POINT_LEFT, POINT_RIGHT
};

static const unsigned int SCORES[MAX_LINE_LENGTH - MIN_LINE_LENGTH + 1] = {
   10, 12, 18, 28, 42, 82, 108, 138, 172, 210
};


/* ======= Structures ======= */

struct GameData {
   Difficulty difficulty;

   size_t colors_count;
   const Cell *colors;

   size_t next_balls_count;
   Cell *next_balls;

   unsigned int field_width;
   unsigned int field_height;
   Cell *field;

   unsigned int score;
   bool game_over;
};

/* ======= Private Function Prototypes ======= */

/**
 * @brief Places the next set of balls on the game field
 *
 * @param data Pointer to the game data structure
 */
static void place_next_balls(GameData *data);

/**
 * @brief Generates the next set of balls for the game
 *
 * @param data Pointer to the game data structure
 */
static void generate_next_balls(GameData *data);

/**
 * @brief Checks for lines of balls on the game field and updates the score
 *
 * @param data Pointer to the game data structure
 *
 * @return true if lines were found and cleared, false otherwise
 */
static bool check_lines(GameData *data);

/**
 * @brief Checks for horizontal lines of balls on the game field
 *
 * @param data Pointer to the game data structure
 * @param to_delete Pointer to a boolean indicating if lines should be deleted
 */
static void check_h_lines(GameData *data, bool *to_delete);

/**
 * @brief Checks for vertical lines of balls on the game field
 *
 * @param data Pointer to the game data structure
 * @param to_delete Pointer to a boolean indicating if lines should be deleted
 */
static void check_v_lines(GameData *data, bool *to_delete);

/**
 * @brief Checks for diagonal lines (top-left to bottom-right)
 *
 * @param data Pointer to the game data structure
 * @param to_delete Array marking cells to be cleared
 */
static void check_d_lines(GameData *data, bool *to_delete);

/**
 * @brief Checks for diagonal lines (bottom-left to top-right)
 *
 * @param data Pointer to the game data structure
 * @param to_delete Array marking cells to be cleared
 */
static void check_b_d_lines(GameData *data, bool *to_delete);

/**
 * @brief Calculates score based on line length
 *
 * @param length Length of the completed line
 *
 * @return Score points for the given line length
 */
static unsigned int score_from_length(unsigned int length);

/**
 * @brief Finds a valid path between two points on the field
 *
 * Uses breadth-first search to find shortest path
 * between start and end points, avoiding occupied cells
 *
 * @param data Pointer to the game data structure
 * @param start Starting point coordinates
 * @param end Ending point coordinates
 *
 * @return Array of points representing the path, NULL if no path exists
 */
static Point *find_path(const GameData *data, Point start, Point end);

/**
 * @brief Calculates distance map for pathfinding
 *
 * Creates a map of distances from start point to all reachable cells
 *
 * @param data Pointer to the game data structure
 * @param start Starting point coordinates
 * @param end Ending point coordinates
 *
 * @return Array of distances, NULL if allocation fails
 */
static int *calculate_distance_map(
   const GameData *data,
   Point start,
   Point end
);

/**
 * @brief Reconstructs path from distance map
 *
 * @param distance Array of distances from pathfinding
 * @param data Pointer to the game data structure
 * @param end End point coordinates
 *
 * @return Array of points representing the path, NULL if allocation fails
 */
static Point *calculate_path(
   int *distance,
   const GameData *data,
   Point end
);

/**
 * @brief Checks if the game is over
 *
 * @param data Pointer to the game data structure
 */
static void check_game_over(GameData *data);

/* ======= Public Functions ======= */

GameData *new_game_data(void) {
   GameData *data = malloc(sizeof(GameData));
   if (data == NULL)
      return NULL;

   data->difficulty = DIFFICULTY_EASY;

   data->colors_count = 0;
   data->colors = NULL;

   data->next_balls_count = 0;
   data->next_balls = NULL;

   data->field_width = 0;
   data->field_height = 0;

   data->field = NULL;

   data->score = 0;
   data->game_over = false;

   return data;
}

int init_new_game(GameData *data, Difficulty difficulty) {
   if (data == NULL)
      return -1;

   size_t colors_count;
   const Cell *colors;
   size_t next_balls_count;
   unsigned int field_width;
   unsigned int field_height;

   switch (difficulty) {
      case DIFFICULTY_EASY:
         colors_count = EASY_COLORS_COUNT;
         colors = EASY_COLORS;
         next_balls_count = EASY_NEXT_BALLS_COUNT;
         field_width = EASY_FIELD_WIDTH;
         field_height = EASY_FIELD_HEIGHT;
         break;
      case DIFFICULTY_MEDIUM:
         colors_count = MEDIUM_COLORS_COUNT;
         colors = MEDIUM_COLORS;
         next_balls_count = MEDIUM_NEXT_BALLS_COUNT;
         field_width = MEDIUM_FIELD_WIDTH;
         field_height = MEDIUM_FIELD_HEIGHT;
         break;
      case DIFFICULTY_HARD:
         colors_count = HARD_COLORS_COUNT;
         colors = HARD_COLORS;
         next_balls_count = HARD_NEXT_BALLS_COUNT;
         field_width = HARD_FIELD_WIDTH;
         field_height = HARD_FIELD_HEIGHT;
         break;
      default:
         return -1;
   }

   Cell *next_balls = calloc(next_balls_count, sizeof(Cell));
   if (next_balls == NULL)
      return -2;

   size_t field_size = field_width * field_height;
   Cell *field = calloc(field_size, sizeof(Cell));
   if (field == NULL) {
      free(next_balls);
      return -2;
   }

   data->difficulty = difficulty;

   data->colors_count = colors_count;
   data->colors = colors;

   data->next_balls_count = next_balls_count;
   free(data->next_balls);
   data->next_balls = next_balls;

   data->field_width = field_width;
   data->field_height = field_height;
   free(data->field);
   data->field = field;

   data->score = 0;
   data->game_over = false;

   randomize();

   generate_next_balls(data);
   place_next_balls(data);

   return 0;
}

size_t get_next_balls_count(const GameData *data) {
   return data == NULL ? 0 : data->next_balls_count;
}

const Cell *get_next_balls(const GameData *data) {
   return data == NULL ? NULL : data->next_balls;
}

unsigned int get_field_width(const GameData *data) {
   return data == NULL ? 0 : data->field_width;
}

unsigned int get_field_height(const GameData *data) {
   return data == NULL ? 0 : data->field_height;
}

const Cell *get_field(const GameData *data) {
   return data == NULL ? NULL : data->field;
}

unsigned int get_score(const GameData *data) {
   return data == NULL ? 0 : data->score;
}

bool game_over(const GameData *data) {
   return data == NULL ? false : data->game_over;
}

void free_game_data(GameData *data) {
   if (data == NULL)
      return;
   free(data->next_balls);
   free(data->field);
   free(data);
}

Point *move_ball(GameData *data, Point start, Point end) {
   if (
      data == NULL ||
      data->colors == NULL ||
      data->next_balls == NULL ||
      data->field == NULL ||
      point_equal(start, end)
   )
      return NULL;

   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;

   size_t start_index = POINT_TO_IDX(start, field_width);
   size_t end_index = POINT_TO_IDX(end, field_width);

   if(
      field == NULL ||
      start.x < 0 ||
      (unsigned int)start.x >= field_width ||
      start.y < 0 ||
      (unsigned int)start.y >= field_height ||
      end.x < 0 ||
      (unsigned int)end.x >= field_width ||
      end.y < 0 ||
      (unsigned int)end.y >= field_height ||
      field[start_index] == CELL_EMPTY ||
      field[end_index] != CELL_EMPTY
   )
      return NULL;

   Point *path = find_path(data, start, end);
   if (path == NULL)
      return NULL;

   field[end_index] = field[start_index];
   field[start_index] = CELL_EMPTY;

   if (!check_lines(data))
      place_next_balls(data);

   return path;
}

size_t path_size(const GameData *data, const Point *path, Point end) {
   if (
      data == NULL ||
      data->field == NULL ||
      path == NULL
   )
      return 0;

   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   size_t field_size = field_width * field_height;

   if (
      end.x < 0 ||
      (unsigned int)end.x >= field_width ||
      end.y < 0 ||
      (unsigned int)end.y >= field_height
   )
      return 0;

   for (size_t i = 0; i < field_size; i++)
      if (point_equal(path[i], end))
         return i + 1;
   return 0;
}

/* ======= Private Functions ======= */

static void place_next_balls(GameData *data) {
   size_t next_balls_count = data->next_balls_count;
   Cell *next_balls = data->next_balls;
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;
   size_t field_size = field_width * field_height;

   // TODO: Optimize this inefficient algorithm
   size_t free_cells_count = 0;
   for (size_t i = 0; i < field_size; i++) {
      if (field[i] == CELL_EMPTY)
         free_cells_count++;
   }

   for (size_t i = 0; i < next_balls_count; i++) {
      if (free_cells_count == 0)
         break;

      size_t random_free_cell = randi_range(0, free_cells_count - 1);
      size_t current_free_cell = 0;
      for (size_t j = 0; j < field_size; j++) {
         if (field[j] != CELL_EMPTY)
            continue;
         if (current_free_cell == random_free_cell) {
            field[j] = next_balls[i];
            free_cells_count--;
            break;
         }
         current_free_cell++;
      }
   }
   generate_next_balls(data);
   check_lines(data);
   check_game_over(data);
}

static void generate_next_balls(GameData *data) {
   size_t color_count = data->colors_count;
   const Cell *colors = data->colors;
   size_t next_balls_count = data->next_balls_count;
   Cell *next_balls = data->next_balls;

   for (size_t i = 0; i < next_balls_count; i++) {
      next_balls[i] = colors[randi_range(0, color_count - 1)];
   }
}

static bool check_lines(GameData *data) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;
   size_t field_size = field_width * field_height;
   unsigned int old_score = data->score;

   bool to_delete[field_size];
   for (size_t i = 0; i < field_size; i++)
      to_delete[i] = false;

   /*
      TODO: Refactor these four similar `check_lines` functions
      Current implementations are overly complex and hard to maintain
   */
   check_h_lines(data, to_delete);
   check_v_lines(data, to_delete);
   check_d_lines(data, to_delete);
   check_b_d_lines(data, to_delete);

   for (size_t i = 0; i < field_size; i++)
      if (to_delete[i])
         field[i] = CELL_EMPTY;

   return data->score != old_score;
}

static void check_h_lines(GameData *data, bool *to_delete) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;

   for (size_t y = 0; y < field_height; y++) {
      unsigned int length = 1;
      Cell previous_cell = CELL_EMPTY;
      for (size_t x = 0; x < field_width; x++) {
         Cell cell = field[XYW_TO_IDX(x, y, field_width)];

         if (previous_cell == cell && cell != CELL_EMPTY) {
            length++;
            if (x != field_width - 1)
               continue;
            x++;
         }

         if (length >= MIN_LINE_LENGTH) {
            for (size_t i = 1; i <= length; i++) {
               to_delete[XYW_TO_IDX(x - i, y, field_width)] = true;
            }
            data->score += score_from_length(length);
         }
         previous_cell = cell;
         length = 1;
      }
   }
}

static void check_v_lines(GameData *data, bool *to_delete) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;

   for (size_t x = 0; x < field_width; x++) {
      unsigned int length = 1;
      Cell previous_cell = CELL_EMPTY;
      for (size_t y = 0; y < field_height; y++) {
         Cell cell = field[XYW_TO_IDX(x, y, field_width)];

         if (previous_cell == cell && cell != CELL_EMPTY) {
            length++;
            if (y != field_height - 1)
               continue;
            y++;
         }

         if (length >= MIN_LINE_LENGTH) {
            for (size_t i = 1; i <= length; i++) {
               to_delete[XYW_TO_IDX(x, y - i, field_width)] = true;
            }
            data->score += score_from_length(length);
         }
         previous_cell = cell;
         length = 1;
      }
   }
}

static void check_d_lines(GameData *data, bool *to_delete) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;

   for (int diff = field_height - 1; diff > -(int)field_width; diff--) {
      unsigned int length = 1;
      Cell previous_cell = CELL_EMPTY;
      for (size_t x = 0; x < field_width; x++) {
         int y = diff + x;
         if (y < 0 || y >= (int)field_height)
            continue;

         Cell cell = field[XYW_TO_IDX(x, y, field_width)];

         if (previous_cell == cell && cell != CELL_EMPTY) {
            length++;
            if (y != (int)field_height - 1 && x != field_width - 1)
               continue;
            x++;
            y++;
         }

         if (length >= MIN_LINE_LENGTH) {
            for (size_t i = 1; i <= length; i++) {
               to_delete[XYW_TO_IDX(x - i, y - i, field_width)] = true;
            }
            data->score += score_from_length(length);
         }
            previous_cell = cell;
            length = 1;
      }
   }
}

static void check_b_d_lines(GameData *data, bool *to_delete) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;

   for (size_t sum = 0; sum < field_width + field_height - 1; sum++) {
      unsigned int length = 1;
      Cell previous_cell = CELL_EMPTY;
      for (size_t x = 0; x < field_width; x++) {
         int y = sum - x;
         if (y < 0 || y >= (int)field_height)
            continue;

         Cell cell = field[XYW_TO_IDX(x, y, field_width)];

         if (previous_cell == cell && cell != CELL_EMPTY) {
            length++;
            if (y != 0 && x != field_width - 1)
               continue;
            x++;
            y--;
         }

         if (length >= MIN_LINE_LENGTH) {
            for (size_t i = 1; i <= length; i++) {
               to_delete[XYW_TO_IDX(x - i, y + i, field_width)] = true;
            }
            data->score += score_from_length(length);
         }
            previous_cell = cell;
            length = 1;
      }
   }
}

static unsigned int score_from_length(unsigned int length) {
   if (length < MIN_LINE_LENGTH)
      return 0;
   if (length > MAX_LINE_LENGTH)
      return SCORES[MAX_LINE_LENGTH - MIN_LINE_LENGTH];
   return SCORES[length - MIN_LINE_LENGTH];
}

static Point *find_path(const GameData *data, Point start, Point end) {
   int *distance = calculate_distance_map(data, start, end);
   if (distance == NULL)
      return NULL;

   Point *path = calculate_path(distance, data, end);
   free(distance);
   return path;
}

static int *calculate_distance_map(
   const GameData *data,
   Point start,
   Point end
) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;
   size_t field_size = field_width * field_height;

   int *distance = calloc(field_size, sizeof(int));
   if (distance == NULL)
      return NULL;
   for (size_t i = 0; i < field_size; i++)
      distance[i] = -1;

   Point queue[field_size];
   size_t front = 0, back = 0;

   distance[POINT_TO_IDX(start, field_width)] = 0;
   queue[back] = start;
   back++;

   for (; front < back; front++) {
      Point cell = queue[front];
      unsigned int cell_dist = distance[POINT_TO_IDX(cell, field_width)];
      if (point_equal(cell, end))
         return distance;

      for (size_t i = 0; i < PATH_DIRECTION_COUNT; i++) {
         Point next_cell = point_add(cell, PATH_DIRECTIONS[i]);

         if (next_cell.x < 0 || field_width <= (unsigned int)next_cell.x ||
            next_cell.y < 0 || field_height <= (unsigned int)next_cell.y ||
            field[POINT_TO_IDX(next_cell, field_width)] != CELL_EMPTY
            || distance[POINT_TO_IDX(next_cell, field_width)] != -1
         )
            continue;

         distance[POINT_TO_IDX(next_cell, field_width)
         ] = cell_dist + 1;
         queue[back] = next_cell;
         back++;
      }
   }
   free(distance);
   return NULL;
}

static Point *calculate_path(
   int *distance,
   const GameData *data,
   Point end
) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;

   size_t path_size = distance[POINT_TO_IDX(end, field_width)] + 1;
   Point *path = calloc(path_size, sizeof(Point));
   if (path == NULL)
      return NULL;

   Point cell = end;
   for (size_t i = 0; i < path_size; i++) {
      path[path_size - (i + 1)] = cell;

      for (size_t j = 0; j < PATH_DIRECTION_COUNT; j++) {
         Point next_cell = point_add(cell, PATH_DIRECTIONS[j]);
         size_t next_cell_index = POINT_TO_IDX(next_cell, field_width);

         if (next_cell.x < 0 || field_width <= (unsigned int)next_cell.x ||
            next_cell.y < 0 || field_height <= (unsigned int)next_cell.y ||
            distance[next_cell_index] == -1
         )
            continue;

         if (
            distance[next_cell_index] < distance[
               POINT_TO_IDX(cell,field_width)
            ]
         ) {
            cell = next_cell;
            break;
         }
      }
   }
   return path;
}

static void check_game_over(GameData *data) {
   unsigned int field_width = data->field_width;
   unsigned int field_height = data->field_height;
   Cell *field = data->field;
   size_t field_size = field_width * field_height;

   for (size_t i = 0; i < field_size; i++)
      if (field[i] == CELL_EMPTY) {
         data->game_over = false;
         return;
      }
   data->game_over = true;
}
