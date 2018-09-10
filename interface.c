/***** interface.c ************************************************
 * Description: Routine for gathering arguments from the command
 *              line.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Sun Jun 20 13:12:10 2004.
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "interface.h"
#include "eprintf.h"

Args *args;

Args *getArgs(int argc, char *argv[]){
  char c;
  char *optString = "hvfrit:n:";

  args = (Args *)emalloc(sizeof(Args));
  args->h = 0;
  args->v = 0;
  args->e = 0;
  args->t = 0;    /* theta */
  args->n = 0;
  args->i = 0;    /* individual spectra? */
  args->r = 0;    /* print raw counts */
  args->f = 0;    /* fold spectrum */
  args->a = 0;    /* hidden option */
  args->s = 0;    /* hidden option */
  args->m = 0;    /* hidden option */
  args->f = 0;    /* hidden option */
  args->c = 0;    /* hidden option */
  c = getopt(argc, argv, optString);
  while(c != -1){
    switch(c){
    case 't':
      args->t = atof(optarg);
      break;
    case 'i':                          /* individual spectra */
      args->i = 1;
      break;
    case 'r':                          /* raw counts */
      args->r = 1;
      break;
    case 'n':
      args->n = atoi(optarg);           /* sample size */
      break;
    case 'f':                           /* fold spectrum */
      args->f = 1;
      break;
    case '?':                           /* fall-through is intentional */
    case 'h':                           /* print help */
      args->h = 1;
      break;
    case 'v':                           /* print version */
      args->v = 1;
      break;
    default:
      printf("# unknown argument: %c\n",c);
      args->e = 1;
      return args;
    }
    c = getopt(argc, argv, optString);
  }
  args->inputFiles = argv + optind;
  args->numInputFiles = argc - optind;
  return args;
}


void printUsage(char *version){
  printf("Usage: %s [options] [inputFiles]\n",progname());
  printf("Compute the site frequency spectrum from Hudson's ms or from its theoretical expectation\n");
  printf("Example 1: ms 5 100000 -t 10 | sfs\n");
  printf("Example 2: sfs -t 10 -n 5\n");
  printf("Options:\n");
  printf("\t[-t <NUM> theta; default: infer from input]\n");
  printf("\t[-n <NUM> sample size; default: infer from input]\n");
  printf("\t[-f fold spectrum]\n");
  printf("\t[-r raw counts; default: average counts]\n");
  printf("\t[-i print individual spectra; default: average across all spectra]\n");
  printf("\t[-h print this help message and exit]\n");
  printf("\t[-v print program information and exit]\n");
  exit(0);
}

void printSplash(char *version){
  printf("%s %s\n",progname(),version);
  printf("Written by Bernhard Haubold.\n");
  printf("Distributed under the GNU General Public License.\n");
  printf("Please send bug reports to haubold@evolbio.mpg.de\n");
  exit(0);
}
