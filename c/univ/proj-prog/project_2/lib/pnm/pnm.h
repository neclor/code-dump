/**
 * @file pnm.h
 * @brief Header file for handling PNM images library.
 *
 * Supported formats:
 * - PBM (Portable Bitmap)
 * - PGM (Portable Graymap)
 * - PPM (Portable Pixmap)
 *
 * @author Pavlov Aleksandr (s2400691)
 * @date 24.03.2025
 * @version 2.0.0
*/

#ifndef _PNM_H
#define _PNM_H

#include <stdint.h>

/* ======= Constants ======= */

#define PBM_MAX_VALUE 1
#define PGM_MAX_VALUE 255
#define PPM_MAX_VALUE 65535

#define PNM_SUCCESS 0
#define PNM_INVALID_FILENAME -1

#define LOAD_PNM_MEMORY_ERROR -2
#define LOAD_PNM_DECODE_ERROR -3

#define WRITE_PNM_FILE_MANIPULATION_ERROR -2

/* ======= Enums ======= */

/**
 * @brief Enum for PNM image formats.
 */
typedef enum FormatPNM_t {
   FORMAT_PBM,
   FORMAT_PGM,
   FORMAT_PPM
} FormatPNM;

/* ======= Structures ======= */

/**
 * @brief Structure representing a PNM image.
 */
typedef struct PNM_t PNM;

/* ======= Function Prototypes ======= */

/**
 * @brief Retrieves the format of a PNM image.
 *
 * @param image Pointer to the PNM image.
 *
 * @pre image != NULL
 *
 * @return
 *     Format of the image
 *    -1: image == NULL
 */
FormatPNM get_format(PNM *image);

/**
 * @brief Retrieves the width of a PNM image.
 *
 * @param image Pointer to the PNM image.
 *
 * @pre image != NULL
 *
 * @return
 *     Width of the image
 *     0: image == NULL
 */
unsigned int get_width(PNM *image);

/**
 * @brief Retrieves the height of a PNM image.
 *
 * @param image Pointer to the PNM image.
 *
 * @pre image != NULL
 *
 * @return
 *     Height of the image
 *     0: image == NULL
 */
unsigned int get_height(PNM *image);

/**
 * @brief Retrieves the maximum pixel value of a PNM image.
 *
 * @param image Pointer to the PNM image.
 *
 * @pre image != NULL
 *
 * @return
 *     Maximum pixel value
 *     0: image == NULL
 */
uint16_t get_max_value(PNM *image);

/**
 * @brief Retrieves the pixel data of a PNM image.
 *
 * @param image Pointer to the PNM image.
 *
 * @pre image != NULL
 *
 * @return
 *     Pointer to the pixel data
 *     NULL : image == NULL
 */
uint16_t *get_data(PNM *image);

/**
 * @brief Sets the properties of a PNM image.
 *
 * @param image Pointer to the PNM image.
 * @param format Format of the image.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param max_value Maximum pixel value.
 * @param data Pointer to the pixel data.
 *
 * @pre image != NULL
 */
void set_pnm(
   PNM *image,
   FormatPNM format,
   unsigned int width,
   unsigned int height,
   uint16_t max_value,
   uint16_t *data
);

/**
 * @brief Frees the memory allocated for a PNM image.
 *
 * @param image Pointer to the pointer of the PNM image to free.
 *
 * @pre image != NULL, *image != NULL
 */
void free_pnm(PNM **image);

/**
 * @brief Loads a PNM image from a file.
 *
 * @param image Pointer to store the loaded PNM image.
 * @param filename Path to the file to load.
 *
 * @pre image != NULL, filename != NULL
 *
 * @return
 *     0: Success
 *    -2: Invalid filename
 *    -1: Memory allocation failure
 *    -3: Decode error
 *    -4: Invalid argument
 */
int load_pnm(PNM **image, const char *filename);

/**
 * @brief Writes a PNM image to a file.
 *
 * @param image Pointer to the PNM image to write.
 * @param filename Path to the file to write to.
 *
 * @pre image != NULL, filename != NULL
 *
 * @return
 *     0: Success
 *    -1: Invalid filename
 *    -2: Writing file error
 */
int write_pnm(PNM *image, const char *filename);

#endif // _PNM_H
