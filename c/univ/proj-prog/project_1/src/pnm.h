/**
 * @author: Pavlov Aleksandr s2400691
 * @date: 05.03.2025
 * @projet: INFO0030 Projet 1
*/


#ifndef _PNM_H
#define _PNM_H 1


#define PNM_LOAD_SUCCESS 0
#define PNM_LOAD_MEMORY_ERROR -1
#define PNM_LOAD_INVALID_FILENAME -2
#define PNM_LOAD_DECODE_ERROR -3

#define PNM_WRITE_SUCCESS 0
#define PNM_WRITE_INVALID_FILENAME -1
#define PNM_WRITE_FILE_MANIPULATION_ERROR -2


typedef enum FormatPNM_t {
    FORMAT_PBM,
    FORMAT_PGM,
    FORMAT_PPM
} FormatPNM;

typedef struct PNM_t PNM;


/**
 * Converts a string to the corresponding FormatPNM enum value.
 *
 * @param format (FormatPNM*): Pointer to store the resulting enum value.
 * @param format_string (const char*): String representation of the format.
 *
 * @pre: format != NULL
 *
 * @return:
 *     0 Success
 *     1 If the string does not match any format
 */
int str_to_format(const char *format_string, FormatPNM *format);

/**
 * Determines the file format based on the filename extension and sets
 * the corresponding FormatPNM enum value.
 *
 * @param format (FormatPNM*): Pointer to store the resulting enum value.
 * @param filename (const char*): Name of the file to analyze.
 *
 * @pre: format != NULL
 *
 * @return:
 *     0 Success
 *     1 If file extension does not match any format
 */
int file_extension_to_format(const char *filename, FormatPNM *format);

/**
 * Frees the memory allocated for a PNM image.
 *
 * @param image (PNM**): A double pointer to a PNM image that will be
 *                       deallocated. The pointer is set to NULL after
 *                       deallocation.
 */
void free_pnm(PNM **image);

/**
 * Loads a PNM image from a file.
 *
 * @param image (PNM**): the address of a pointer on PNM to write the address
 *                       of the loaded image to.
 * @param filename (const char*): the path to the file containing the image.
 *
 * @pre: image != NULL, filename != NULL
 * @post: image points to the image loaded from the file.
 *
 * @return:
 *     0 Success
 *    -1 Memory allocation error
 *    -2 Malformed file name
 *    -3 Malformed file content
 */
int load_pnm(PNM **image, const char* filename);

/**
 * Saves a PNM image to a file.
 *
 * @param image (PNM*): a pointer to PNM.
 * @param filename (const char*): the path to the destination file.
 *
 * @pre: image != NULL, filename != NULL
 * @post: the file filename contains the PNM image.
 *
 * @return:
 *     0 Success
 *    -1 Malformed file name
 *    -2 Error while manipulating the file
 */
int write_pnm(PNM *image, const char* filename);

#endif // pnm.h
