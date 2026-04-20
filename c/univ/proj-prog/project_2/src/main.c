/**
 * @file main.c
 * @brief A program to manipulate PNM format files by applying various filters.
 *
 * Supported formats:
 * - PBM (Portable Bitmap)
 * - PGM (Portable Graymap)
 * - PPM (Portable Pixmap)
 *
 * @author Pavlov Aleksandr (s2400691)
 * @date 24.03.2025
 * @version 1.0.0
 */

#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "pnm.h"
#include "filter.h"


#define VERSION "1.0.0"
#define AUTHORS "Pavlov Aleksandr (s2400691)"


enum {
   GETOPT_HELP_CHAR = (CHAR_MIN - 2),
   GETOPT_VERSION_CHAR = (CHAR_MIN - 3),
};

static struct option const longopts[] = {
   {"input", required_argument, NULL, 'i'},
   {"output", required_argument, NULL, 'o'},
   {"filtre", required_argument, NULL, 'f'},
   {"parametres", required_argument, NULL, 'p'},
   {"help", no_argument, NULL, GETOPT_HELP_CHAR},
   {"version", no_argument, NULL, GETOPT_VERSION_CHAR},
   {NULL, no_argument, NULL, 0},
};

const char *program_name;

/* ======= Function Prototypes ======= */

/**
 * @brief Displays help about using the command and exits the program.
 *
 * This function prints the usage instructions for the program, including
 * details about the available options and filters.
 *
 * @param status Exit status to terminate the program with.
 */
static void usage(int status);

/* ======= Functions ======= */

/**
 * @brief Entry point of the program.
 *
 * Parses command-line arguments, loads the input PNM file, applies the
 * specified filter, and writes the result to the output file.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *
 * @return int Exit status of the program.
 */
int main(int argc, char **argv) {
   program_name = argv[0];

   const char *input_filename = NULL;
   const char *output_filename = NULL;
   const char *filter_string = NULL;
   const char *parameter_string = NULL;

   int optc;
   while ((optc = getopt_long(argc, argv, "i:o:f:p:", longopts, NULL)) != -1) {
      switch (optc) {
         case 'i':
            input_filename = optarg;
            break;
         case 'o':
            output_filename = optarg;
            break;
         case 'f':
            filter_string = optarg;
            break;
         case 'p':
            parameter_string = optarg;
            break;
         case GETOPT_HELP_CHAR:
            usage(EXIT_SUCCESS);
            break;
         case GETOPT_VERSION_CHAR:
            fprintf(stdout, "%s %s\n\nWritten by %s.\n",
               program_name, VERSION, AUTHORS);
            exit(EXIT_SUCCESS);
            break;
         default:
            usage(EXIT_FAILURE);
      }
   }

   if (input_filename == NULL) {
      fprintf(stderr, "%s: missing '-i' argument\n", program_name);
      usage(EXIT_FAILURE);
   } else if (output_filename == NULL) {
      fprintf(stderr, "%s: missing '-o' argument\n", program_name);
      usage(EXIT_FAILURE);
   }

   PNM *image = NULL;

   switch (load_pnm(&image, input_filename)) {
      case PNM_SUCCESS:
         break;
      case PNM_INVALID_FILENAME:
         fprintf(stderr, "%s: invalid filename '%s': ",
            program_name, input_filename);
         perror("");
         return EXIT_FAILURE;
      case LOAD_PNM_MEMORY_ERROR:
         fprintf(stderr, "%s: ", program_name);
         perror("");
         return EXIT_FAILURE;
      case LOAD_PNM_DECODE_ERROR:
         fprintf(stderr, "%s: '%s': decode error\n",
            program_name, input_filename);
         return EXIT_FAILURE;
      default:
         fprintf(stderr, "%s: error:", program_name);
         perror("");
         return EXIT_FAILURE;
   }

   int result_code;
   if (filter_string == NULL) {
      result_code = FILTER_SUCCESS;
   } else if (!strcasecmp(filter_string, "retournement")) {
      result_code = turnaround(image);
   } else if (!strcasecmp(filter_string, "monochrome")) {
      result_code = monochrome(image, parameter_string);
   } else if (!strcasecmp(filter_string, "negatif")) {
      result_code = negative(image);
   } else if (!strcasecmp(filter_string, "gris")) {
      result_code = fifty_shades_of_grey(image, parameter_string);
   } else if (!strcasecmp(filter_string, "NB")) {
      result_code = black_and_white(image, parameter_string);
   } else {
      fprintf(stderr, "%s: invalid filter name '%s'\n",
         program_name, filter_string);
      free_pnm(&image);
      usage(EXIT_FAILURE);
   }

   switch(result_code) {
      case FILTER_SUCCESS:
         break;
      case FILTER_WRONG_IMAGE_FORMAT:
         fprintf(stderr, "%s: incompatible filter and image format\n",
            program_name);
         free_pnm(&image);
         usage(EXIT_FAILURE);
         break;
      case FILTER_INVALID_PARAMETER:
         if (parameter_string == NULL) {
            fprintf(stderr, "%s: missing '-p' argument\n", program_name);
         } else {
            fprintf(stderr, "%s: '%s': invalid argument\n",
               program_name, parameter_string);
         }
         free_pnm(&image);
         usage(EXIT_FAILURE);
         break;
      default:
         fprintf(stderr, "%s: error: ", program_name);
         perror("");
         free_pnm(&image);
         return EXIT_FAILURE;
   }

   int ok = 1;
   switch (write_pnm(image, output_filename)) {
      case PNM_SUCCESS:
         break;
      case PNM_INVALID_FILENAME:
         fprintf(stderr, "%s: invalid filename '%s': ",
            program_name, output_filename);
         perror("");
         ok = 0;
         break;
      case WRITE_PNM_FILE_MANIPULATION_ERROR:
         fprintf(stderr, "%s: '%s': file manipulation error: ",
            program_name, output_filename);
         perror("");
         ok = 0;
         break;
      default:
         fprintf(stderr, "%s: error: ", program_name);
         perror("");
         ok = 0;
   }

   free_pnm(&image);

   return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void usage(int status) {
   if (status != EXIT_SUCCESS) {
      fprintf(stderr, "Try '%s --help' for more information.\n",
         program_name);
   } else {
      printf("Usage: %s -i SOURCE [-f FILTER] [-p PARAM] -o DEST\n",
         program_name);
      fputs("\
Manipulates PNM format files.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -i, --input=FILE             specify input file (.ppm, .pbm, .pgm)\n\
  -o, --output=FILE            specify output file (.ppm, .pbm, .pgm)\n\
  -f, --filter=FILTER          specify filter to apply:\n\
                                 retournement  (NO PARAM)\n\
                                 monochrome    (PARAM: r, v, b)\n\
                                 negatif       (NO PARAM)\n\
                                 gris          (PARAM: 1, 2)\n\
                                 NB            (PARAM: 0 - 255)\n\
  -p, --parameter=PARAM        specify parameter for the filter (if required)\n\
      --help                   display this help and exit\n\
      --version                output version information and exit\n\
", stdout);
   }
   exit(status);
}

