/**
 * @author: Pavlov Aleksandr s2400691
 * @date: 05.03.2025
 * @projet: INFO0030 Projet 1
*/


#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"


#define PROGRAM_NAME "./pnm"
#define VERSION "1.2.0"
#define AUTHORS "Pavlov Aleksandr (s2400691)"


enum {
   GETOPT_HELP_CHAR = (CHAR_MIN - 2),
   GETOPT_VERSION_CHAR = (CHAR_MIN - 3),
};

static struct option const longopts[] = {
   {"format", required_argument, NULL, 'f'},
   {"input", required_argument, NULL, 'i'},
   {"output", required_argument, NULL, 'o'},
   {"help", no_argument, NULL, GETOPT_HELP_CHAR},
   {"version", no_argument, NULL, GETOPT_VERSION_CHAR},
   {NULL, no_argument, NULL, 0},
};

const char *program_name;


// ======= Prototypes =======


static void usage(int status);


// ======= Code =======


static void usage(int status) {
   if (status != EXIT_SUCCESS) {
      fprintf(stderr, "Try '%s --help' for more information.\n",
         program_name);
   } else {
      printf("Usage: %s [-f FORMAT] -i SOURCE -o DEST\n", program_name);
      fputs("\
Manipulates PNM format files.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -f, --format=FORMAT          specify format FORMAT={PBM,PGM,PPM}\n\
  -i, --input=FILE             specify input file {.ppm,.pbm,.pgm}\n\
  -o, --output=FILE            specify output file {.ppm,.pbm,.pgm}\n\
      --help        display this help and exit\n\
      --version     output version information and exit\n\
", stdout);
   }
   exit(status);
}

int main(int argc, char **argv) {
   const char *format_string = NULL;
   const char *input_filename = NULL;
   const char *output_filename = NULL;

   FormatPNM arg_format;
   FormatPNM input_file_format;
   FormatPNM output_file_format;
   PNM *image = NULL;

   program_name = argv[0];

   int optc;
   while ((optc = getopt_long(argc, argv, "f:i:o:", longopts, NULL)) != -1) {
      switch (optc) {
         case 'f':
            format_string = optarg;
            break;
         case 'i':
            input_filename = optarg;
            break;
         case 'o':
            output_filename = optarg;
            break;
         case GETOPT_HELP_CHAR:
            usage(EXIT_SUCCESS);
            break;
         case GETOPT_VERSION_CHAR:
            fprintf(stdout, "%s %s\n\nWritten by %s.\n",
               PROGRAM_NAME, VERSION, AUTHORS);
            exit(EXIT_SUCCESS);
            break;
         default:
            usage(EXIT_FAILURE);
      }
   }

   if (format_string == NULL) {
      fprintf(stderr, "%s: missing '-f' flag\n", program_name);
      usage(EXIT_FAILURE);
   }
   if (input_filename == NULL) {
      fprintf(stderr, "%s: missing '-i' flag\n", program_name);
      usage(EXIT_FAILURE);
   }
   if (output_filename == NULL) {
      fprintf(stderr, "%s: missing '-o' flag\n", program_name);
      usage(EXIT_FAILURE);
   }

   if (str_to_format(format_string, &arg_format) != 0) {
      fprintf(stderr, "%s: unrecognized format '%s' specified with -f\n",
         program_name, format_string);
      usage(EXIT_FAILURE);
   }
   if (file_extension_to_format(input_filename, &input_file_format) != 0) {
      fprintf(stderr, "%s: unrecognized file extension '%s'\n",
         program_name, input_filename);
      usage(EXIT_FAILURE);
   }
   if (file_extension_to_format(output_filename, &output_file_format) != 0) {
      fprintf(stderr, "%s: unrecognized file extension '%s'\n",
         program_name, output_filename);
      usage(EXIT_FAILURE);
   }

   if (arg_format != input_file_format || input_file_format != output_file_format) {
      fprintf(stderr, "%s: file and/or argument formats do not match\n",
         program_name);
      exit(EXIT_FAILURE);
   }

   switch (load_pnm(&image, input_filename)) {
      case PNM_LOAD_SUCCESS:
         break;
      case PNM_LOAD_MEMORY_ERROR:
         fprintf(stderr, "%s: ", program_name);
         perror("");
         return EXIT_FAILURE;
      case PNM_LOAD_INVALID_FILENAME:
         fprintf(stderr, "%s: invalid filename '%s': ",
            program_name, input_filename);
         perror("");
         return EXIT_FAILURE;
      case PNM_LOAD_DECODE_ERROR:
         fprintf(stderr, "%s: '%s': decode error\n",
            program_name, input_filename);
         return EXIT_FAILURE;
      default:
         perror("");
         return EXIT_FAILURE;
   }

   int ok = 1;
   switch (write_pnm(image, output_filename)) {
      case PNM_WRITE_SUCCESS:
         break;
      case PNM_WRITE_INVALID_FILENAME:
         ok = 0;
         fprintf(stderr, "%s: invalid filename '%s': ",
            program_name, output_filename);
         perror("");
         break;
      case PNM_WRITE_FILE_MANIPULATION_ERROR:
         ok = 0;
         fprintf(stderr, "%s: '%s': file manipulation error: ",
            program_name, output_filename);
         perror("");
         break;
      default:
         ok = 0;
         perror("");
   }

   free_pnm(&image);

   return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
