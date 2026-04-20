/**
 * @file pnm.c
 * @brief Implementation of functions for handling PNM images.
 *
 * @author Pavlov Aleksandr (s2400691)
 * @date 24.03.2025
*/

#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "pnm.h"

/* ======= Constants ======= */

#define INVALID_FILENAME_CHARACTERS "\\:*?\"<>|"

/* ======= Structures ======= */

struct PNM_t {
   FormatPNM format;
   unsigned int width;
   unsigned int height;
   uint16_t max_value;
   uint16_t *data;
};

/* ======= Internal Function Prototypes ======= */

/**
 * @brief Reads the header of a PNM file.
 *
 * @param file Pointer to the file to read from.
 * @param format Pointer to store the format of the PNM file.
 * @param width Pointer to store the width of the image.
 * @param height Pointer to store the height of the image.
 * @param max_value Pointer to store the maximum pixel value.
 *
 * @pre file != NULL, format != NULL, width != NULL, height != NULL,
 *      max_value != NULL
 *
 * @return
 *     0 on success
 *    -1 magic string is invalid
 *    -2 width or height is invalid
 *    -3 max_value is invalid
 */
static int read_header(
   FILE *file,
   FormatPNM *format,
   unsigned int *width,
   unsigned int *height,
   uint16_t *max_value
);

/**
 * @brief Reads the pixel data from a PNM file.
 *
 * @param file Pointer to the file to read from.
 * @param max_value Maximum pixel value allowed.
 * @param data_count Number of pixels to read.
 * @param data Pointer to the buffer to store the pixel data.
 *
 * @pre file != NULL, data != NULL
 *
 * @return
 *     0 on success
 *    -1 if pixel data is invalid
 */
static int read_data(
   FILE *file,
   uint16_t max_value,
   size_t data_count,
   uint16_t *data
);

/**
 * @brief Writes the header of a PNM file.
 *
 * @param file Pointer to the file to write to.
 * @param image Pointer to the PNM image structure.
 *
 * @pre file != NULL, image != NULL
 *
 * @return
 *     0 on success
 *    -1 on error
 */
static int write_header(FILE *file, PNM *image);

/**
 * @brief Writes the pixel data to a PNM file.
 *
 * @param file Pointer to the file to write to.
 * @param image Pointer to the PNM image structure.
 *
 * @pre file != NULL, image != NULL
 *
 * @return
 *     0 on success
 *    -1 on error
 */
static int write_data(FILE *file, PNM *image);

/**
 * @brief Checks if a filename contains invalid characters.
 *
 * @param filename Pointer to the filename string.
 *
 * @pre filename != NULL
 *
 * @return
 *     0 valid
 *     1 invalid characters are found
 */
static int check_invalid_characters(const char *filename);

/**
 * @brief Determines the PNM format based on the file extension.
 *
 * @param filename Pointer to the filename string.
 * @param format Pointer to store the determined format.
 *
 * @pre filename != NULL, format != NULL
 *
 * @return
 *     0 on success
 *    -2 file extension is invalid
 */
static int file_extension_to_format(const char *filename, FormatPNM *format);

/**
 * @brief Converts a magic string to a PNM format.
 *
 * @param magic_string Pointer to the magic string.
 * @param format Pointer to store the determined format.
 *
 * @pre magic_string != NULL, format != NULL
 *
 * @return
 *     0 on success
 *    -1 magic string is invalid
 */
static int magic_str_to_format(const char *magic_str, FormatPNM *format);

/**
 * @brief Converts a PNM format to its corresponding magic string.
 *
 * @param format The PNM format.
 * @param magic_str Pointer to store the corresponding magic string.
 *
 * @pre magic_str != NULL
 *
 * @return
 *     0 on success
 *    -1 format is invalid
 */
static int format_to_magic_str(FormatPNM format, const char **magic_str);

/**
 * @brief Skips comments and whitespace in a PNM file.
 *
 * @param file Pointer to the file to read from.
 *
 * @pre file != NULL
 */
static void skip_comments(FILE *file);

/**
 * @brief Reads an unsigned integer from a PNM file.
 *
 * @param file Pointer to the file to read from.
 * @param value Pointer to store the read value.
 *
 * @pre file != NULL, value != NULL
 *
 * @return
 *     0 on success
 *    -1 on error
 */
static int read_unsigned_int(FILE *file, unsigned int *value);

/**
 * @brief Reads an unsigned integer using fscanf.
 *
 * @param file Pointer to the file to read from.
 * @param value Pointer to store the read value.
 *
 * @pre file != NULL, value != NULL
 *
 * @return
 *     0 on success
 *    -1 if the value is invalid
 *    -2 if the value is out of range
 */
static int fscanf_unsigned_int(FILE *file, unsigned int *value);

/* ======= External Functions ======= */

FormatPNM get_format(PNM *image) {
   if (image == NULL) return -1;
   return image->format;
}

unsigned int get_width(PNM *image) {
   if (image == NULL) return 0;
   return image->width;
}

unsigned int get_height(PNM *image) {
   if (image == NULL) return 0;
   return image->height;
}

uint16_t get_max_value(PNM *image) {
   if (image == NULL) return 0;
   return image->max_value;
}

uint16_t *get_data(PNM *image) {
   if (image == NULL) return NULL;
   return image->data;
}

void set_pnm(
   PNM *image,
   FormatPNM format,
   unsigned int width,
   unsigned int height,
   uint16_t max_value,
   uint16_t *data
) {
   if (image == NULL) return;
   image->format = format;
   image->width = width;
   image->height = height;
   image->max_value = max_value;
   image->data = data;
}

void free_pnm(PNM **image) {
   if (image == NULL || *image == NULL) return;
   free((*image)->data);
   free(*image);
   *image = NULL;
}

int load_pnm(PNM **image, const char *filename) {
   if (image == NULL || filename == NULL) return -4;

   FormatPNM file_extension;
   if (file_extension_to_format(filename, &file_extension) != 0) {
      return PNM_INVALID_FILENAME;
   }

   FILE *file = fopen(filename, "r");
   if (file == NULL) return PNM_INVALID_FILENAME;

   FormatPNM format;
   unsigned int width;
   unsigned int height;
   uint16_t max_value;

   if (read_header(file, &format, &width, &height, &max_value) != 0) {
      if (fclose(file) != 0) return -4;
      return LOAD_PNM_DECODE_ERROR;
   }

   if (file_extension != format) {
      if (fclose(file) != 0) return -4;
      return LOAD_PNM_DECODE_ERROR;
   }

   size_t data_count = width * height;
   if (format == FORMAT_PPM) data_count *= 3;

   uint16_t *data = malloc(data_count * sizeof(uint16_t));
   if (data == NULL) {
      if (fclose(file) != 0) return -4;
      return LOAD_PNM_MEMORY_ERROR;
   }

   if (read_data(file, max_value, data_count, data) != 0) {
      free(data);
      if (fclose(file) != 0) return -4;
      return LOAD_PNM_DECODE_ERROR;
   }

   *image = malloc(sizeof(PNM));
   if (*image == NULL) {
      free(data);
      if (fclose(file) != 0) return -4;
      return LOAD_PNM_MEMORY_ERROR;
   }

   set_pnm(*image, format, width, height, max_value, data);

   if (fclose(file) != 0) {
      free_pnm(image);
      return -4;
   }
   return PNM_SUCCESS;
}

int write_pnm(PNM *image, const char *filename) {
   if (image == NULL || filename == NULL) return -4;

   if (check_invalid_characters(filename) != 0) {
      return PNM_INVALID_FILENAME;
   }

   FormatPNM file_extension;
   if (file_extension_to_format(filename, &file_extension) != 0) {
      return PNM_INVALID_FILENAME;
   }
   if (image->format != file_extension) return PNM_INVALID_FILENAME;

   FILE *file = fopen(filename, "w");
   if (file == NULL) return PNM_INVALID_FILENAME;

   if (write_header(file, image) != 0) {
      if (fclose(file) != 0) return WRITE_PNM_FILE_MANIPULATION_ERROR;
      return WRITE_PNM_FILE_MANIPULATION_ERROR;
   }

   if (write_data(file, image) != 0) {
      if (fclose(file) != 0) return WRITE_PNM_FILE_MANIPULATION_ERROR;
      return WRITE_PNM_FILE_MANIPULATION_ERROR;
   }

   if (fclose(file) != 0) return WRITE_PNM_FILE_MANIPULATION_ERROR;
   return PNM_SUCCESS;
}

/* ======= Internal functions ======= */

static int read_header(
   FILE *file,
   FormatPNM *format,
   unsigned int *width,
   unsigned int *height,
   uint16_t *max_value
) {
   skip_comments(file);

   char magic_str[3];
   if (fscanf(file, "%2s", magic_str) != 1) return -1;
   if (magic_str_to_format(magic_str, format) != 0) return -1;

   if (read_unsigned_int(file, width) != 0) return -2;
   if (read_unsigned_int(file, height) != 0) return -2;
   if (width == 0 || height == 0) return -2;

   if (*format == FORMAT_PBM) {
      *max_value = PBM_MAX_VALUE;
      return 0;
   }

   unsigned int new_max_value;
   if (read_unsigned_int(file, &new_max_value) != 0) return -3;

   switch (*format) {
      case FORMAT_PGM:
         if (PGM_MAX_VALUE < new_max_value) return -3;
         break;
      case FORMAT_PPM:
         if (PPM_MAX_VALUE < new_max_value) return -3;
         break;
      default:
         return -3;
   }
   *max_value = new_max_value;
   return 0;
}

static int read_data(
   FILE *file,
   uint16_t max_value,
   size_t data_count,
   uint16_t *data
) {
   for (size_t i = 0; i < data_count; ++i) {
      unsigned int value;
      if (read_unsigned_int(file, &value) != 0) return -1;
      if (max_value < value) return -1;
      data[i] = value;
   }
   return 0;
}

static int write_header(FILE *file, PNM *image) {
   FormatPNM format = image->format;
   unsigned int width = image->width;
   unsigned int height = image->height;
   uint16_t max_value = image->max_value;

   const char *magic_str = NULL;
   if (format_to_magic_str(format, &magic_str) != 0) return -1;
   if (fprintf(file, "%s\n", magic_str) < 0) return -1;
   if (fprintf(file, "%u %u\n", width, height) < 0) return -1;
   if (format != FORMAT_PBM) {
      if (fprintf(file, "%u\n", max_value) < 0) return -1;
   }
   return 0;
}

static int write_data(FILE *file, PNM *image) {
   FormatPNM format = image->format;
   unsigned int width = image->width;
   unsigned int height = image->height;
   uint16_t *data = image->data;

   if (format == FORMAT_PPM) {
      width *= 3;
   }

   for (unsigned int y = 0; y < height; ++y) {
      for (unsigned int x = 0; x < width; ++x) {
         size_t i = y * width + x;
         if (fprintf(file, "%u ", data[i]) < 0) return -1;
      }
      if (fprintf(file, "\n") < 0) return -1;
   }
   return 0;
}

static int check_invalid_characters(const char *string) {
   if (strpbrk(string, INVALID_FILENAME_CHARACTERS) != NULL) return 1;
   return 0;
}

static int file_extension_to_format(const char *filename, FormatPNM *format) {
   int filename_len = strlen(filename);
   if (filename_len < 4) return -2;
   if (filename[filename_len - 4] != '.') return -2;
   const char *extension_string = &(filename[filename_len - 3]);
   if (!strcasecmp(extension_string, "pbm")) {
      *format = FORMAT_PBM;
   } else if (!strcasecmp(extension_string, "pgm")) {
      *format = FORMAT_PGM;
   } else if (!strcasecmp(extension_string, "ppm")) {
      *format = FORMAT_PPM;
   } else {
      return -2;
   }
   return 0;
}

static int magic_str_to_format(const char *magic_str, FormatPNM *format) {
   if (!strcmp(magic_str, "P1")) {
      *format = FORMAT_PBM;
   } else if (!strcmp(magic_str, "P2")) {
      *format = FORMAT_PGM;
   } else if (!strcmp(magic_str, "P3")) {
      *format = FORMAT_PPM;
   } else {
      return -1;
   }
   return 0;
}

static int format_to_magic_str(FormatPNM format, const char **magic_str) {
   switch (format) {
      case FORMAT_PBM:
         *magic_str = "P1";
         return 0;
      case FORMAT_PGM:
         *magic_str = "P2";
         return 0;
      case FORMAT_PPM:
         *magic_str = "P3";
         return 0;
      default:
         return -1;
   }
}

static void skip_comments(FILE *file) {
   char c;
   while ((c = fgetc(file)) != EOF) {
      if isspace(c) continue;
      if (c == '#') {
         while ((c = fgetc(file)) != EOF && c != '\n') {}
      } else {
         ungetc(c, file);
         break;
      }
   }
}

static int read_unsigned_int(FILE *file, unsigned int *value) {
   skip_comments(file);
   if (fscanf_unsigned_int(file, value) != 0) return -1;
   return 0;
}

static int fscanf_unsigned_int(FILE *file, unsigned int *value) {
   long long_value;
   if (fscanf(file, "%ld", &long_value) != 1) return -1;
   if (long_value < 0 || UINT_MAX < long_value) return -2;
   *value = long_value;
   return 0;
}
