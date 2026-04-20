/**
 * @author: Pavlov Aleksandr s2400691
 * @date: 05.03.2025
 * @projet: INFO0030 Projet 1
*/


#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "pnm.h"


#define PBM_MAX_VALUE 1
#define PGM_MAX_VALUE 255


struct PNM_t {
   FormatPNM format;
   unsigned int width;
   unsigned int height;
   uint16_t max_value;

   size_t data_count;
   uint16_t *data;
};


// ======= Prototypes =======


static int create_pnm(
   PNM **image,
   FormatPNM format,
   unsigned int width,
   unsigned int height,
   uint16_t max_value
);

static int read_header(
   FILE *file,
   FormatPNM *format,
   unsigned int *width,
   unsigned int *height,
   uint16_t *max_value
);

static int magic_string_to_format(const char *magic_string, FormatPNM *format);
static void skip_comments(FILE *file);
static int read_uint16(FILE *file, uint16_t *value);
static int read_unsigned_int(FILE *file, unsigned int *value);
static int fscanf_unsigned_int(FILE *file, unsigned int *value);

static int read_data(FILE *file, PNM *image);

static int check_invalid_characters(const char *filename);
static int write_header(FILE *file, PNM *image);
static const char *format_to_magic_string(FormatPNM format);
static int write_data(FILE *file, PNM *image);


// ======= Code =======


int str_to_format(const char *format_string, FormatPNM *format) {
   if (!strcasecmp(format_string, "pbm")) {
      *format = FORMAT_PBM;
   } else if (!strcasecmp(format_string, "pgm")) {
      *format = FORMAT_PGM;
   } else if (!strcasecmp(format_string, "ppm")) {
      *format = FORMAT_PPM;
   } else {
      return 1;
   }
   return 0;
}

int file_extension_to_format(const char *filename, FormatPNM *format) {
   int filename_len = strlen(filename);
   if (filename_len < 4) return 1;
   if (filename[filename_len - 4] != '.') return 1;
   const char *extension_string = &(filename[filename_len - 3]);
   return str_to_format(extension_string, format);
}

void free_pnm(PNM **image) {
   free((*image)->data);
   free(*image);
   *image = NULL;
}

static int create_pnm(
   PNM **image,
   FormatPNM format,
   unsigned int width,
   unsigned int height,
   uint16_t max_value
) {
   *image = malloc(sizeof(PNM));
   if (*image == NULL) return 1;

   PNM *image_pnm = *image;
   image_pnm->format = format;
   image_pnm->width = width;
   image_pnm->height = height;
   image_pnm->max_value = max_value;

   size_t data_count = width * height;
   if (format == FORMAT_PPM) {
      data_count *= 3;
   }

   size_t data_size = data_count * sizeof(uint16_t);
   uint16_t *data = malloc(data_size);
   if (data == NULL) {
      free_pnm(image);
      return 1;
   }

   image_pnm->data_count = data_count;
   image_pnm->data = data;
   return 0;
}

int load_pnm(PNM **image, const char *filename) {
   FormatPNM file_extension;
   if (file_extension_to_format(filename, &file_extension) != 0) {
      return PNM_LOAD_INVALID_FILENAME;
   }

   FILE *file = fopen(filename, "r");
   if (file == NULL) return PNM_LOAD_INVALID_FILENAME;

   FormatPNM format;
   unsigned int width;
   unsigned int height;
   uint16_t max_value;

   if (read_header(file, &format, &width, &height, &max_value) != 0) {
      if (fclose(file) != 0) return PNM_LOAD_INVALID_FILENAME;
      return PNM_LOAD_DECODE_ERROR;
   }

   if (format != file_extension) {
      if (fclose(file) != 0) return PNM_LOAD_INVALID_FILENAME;
      return PNM_LOAD_DECODE_ERROR;
   }

   if (create_pnm(image, format, width, height, max_value) != 0) {
      if (fclose(file) != 0) return PNM_LOAD_INVALID_FILENAME;
      return PNM_LOAD_MEMORY_ERROR;
   }

   if (read_data(file, *image) != 0) {
      free_pnm(image);
      if (fclose(file) != 0) return PNM_LOAD_INVALID_FILENAME;
      return PNM_LOAD_DECODE_ERROR;
   }

   if (fclose(file) != 0) return PNM_LOAD_INVALID_FILENAME;
   return PNM_LOAD_SUCCESS;
}

static int read_header(
   FILE *file,
   FormatPNM *format,
   unsigned int *width,
   unsigned int *height,
   uint16_t *max_value
) {
   skip_comments(file);

   char magic_string[3];
   if (!fscanf(file, "%2s", magic_string)) return 1;
   if (magic_string_to_format(magic_string, format) != 0) return 1;

   if (read_unsigned_int(file, width) != 0) return 1;
   if (read_unsigned_int(file, height) != 0) return 1;

   switch (*format)
   {
      case FORMAT_PBM:
         *max_value = PBM_MAX_VALUE;
         break;
      case FORMAT_PGM:
         if (read_uint16(file, max_value) != 0) return 1;
         if (PGM_MAX_VALUE < *max_value) return 1;
         break;
      case FORMAT_PPM:
         if (read_uint16(file, max_value) != 0) return 1;
         break;
   }
   return 0;
}

static int magic_string_to_format(const char *magic_string, FormatPNM *format) {
   if (strcmp(magic_string, "P1") == 0) {
      *format = FORMAT_PBM;
   } else if (strcmp(magic_string, "P2") == 0) {
      *format = FORMAT_PGM;
   } else if (strcmp(magic_string, "P3") == 0) {
      *format = FORMAT_PPM;
   } else {
      return 1;
   }
   return 0;
}

static void skip_comments(FILE *file) {
   char c;
   while ((c = fgetc(file)) != EOF) {
      if isspace(c) continue;
      if (c == '#') {
         while ((c = fgetc(file)) != EOF && c != '\n') {
         }
      } else {
         ungetc(c, file);
         break;
      }
   }
}

static int read_uint16(FILE *file, uint16_t *value) {
   unsigned int u_value;
   if (read_unsigned_int(file, &u_value) != 0) return 1;
   if (UINT16_MAX < u_value) return 1;
   *value = u_value;
   return 0;
}

static int read_unsigned_int(FILE *file, unsigned int *value) {
   skip_comments(file);
   if (fscanf_unsigned_int(file, value) != 0) return 1;
   return 0;
}

static int fscanf_unsigned_int(FILE *file, unsigned int *value) {
   long long_value;
   if (fscanf(file, "%ld", &long_value) != 1) return 1;
   if (UINT_MAX < long_value || long_value < 0) return 1;
   *value = long_value;
   return 0;
}

static int read_data(FILE *file, PNM *image) {
   uint16_t max_value = image->max_value;
   size_t data_count = image->data_count;
   uint16_t *data = image->data;
   for (size_t i = 0; i < data_count; i++) {
      uint16_t value;
      if (read_uint16(file, &value) != 0) return 1;
      if (max_value < value) return 1;
      data[i] = value;
   }
   return 0;
}

int write_pnm(PNM *image, const char *filename) {
   if (check_invalid_characters(filename) != 0) {
      return PNM_WRITE_INVALID_FILENAME;
   }

   FILE *file = fopen(filename, "w");
   if (file == NULL) return PNM_WRITE_INVALID_FILENAME;

   if (write_header(file, image) != 0) {
      if (fclose(file) != 0) return PNM_WRITE_FILE_MANIPULATION_ERROR;
      return PNM_WRITE_FILE_MANIPULATION_ERROR;
   }

   if (write_data(file, image) != 0) {
      if (fclose(file) != 0) return PNM_WRITE_FILE_MANIPULATION_ERROR;
      return PNM_WRITE_FILE_MANIPULATION_ERROR;
   }

   if (fclose(file) != 0) return PNM_WRITE_FILE_MANIPULATION_ERROR;
   return 0;
}

static int check_invalid_characters(const char *filename) {
   const char *invalid_characters = "\\:*?\"<>|";
   if (strpbrk(filename, invalid_characters) != NULL) return 1;
   return 0;
}

static int write_header(FILE *file, PNM *image) {
   FormatPNM format = image->format;
   unsigned int width = image->width;
   unsigned int height = image->height;
   uint16_t max_value = image->max_value;

   if (fprintf(file, "%s\n", format_to_magic_string(format)) < 0) return 1;
   if (fprintf(file, "%u %u\n", width, height) < 0) return 1;
   if (format != FORMAT_PBM) {
      if (fprintf(file, "%u\n", max_value) < 0) return 1;
   }
   return 0;
}

static const char *format_to_magic_string(FormatPNM format) {
   switch (format) {
      case FORMAT_PBM:
         return "P1";
      case FORMAT_PGM:
         return "P2";
      case FORMAT_PPM:
         return "P3";
      default:
         return NULL;
   }
}

static int write_data(FILE *file, PNM *image) {
   FormatPNM format = image->format;
   unsigned int width = image->width;
   unsigned int height = image->height;
   uint16_t *data = image->data;

   if (format == FORMAT_PPM) {
      width *= 3;
   }

   for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
         size_t i = y * width + x;
         if (fprintf(file, "%u ", data[i]) < 0) return 1;
      }
      if (fprintf(file, "\n") < 0) return 1;
   }
   return 0;
}
