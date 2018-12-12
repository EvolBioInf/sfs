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
  int c;
  char *optString = "hvs:i:";

  args = (Args *)emalloc(sizeof(Args));
  args->i = DEFAULT_I;
  args->s = 0;
  args->h = 0;
  args->v = 0;
  args->e = 0;

  c = getopt(argc, argv, optString);
  while(c != -1){
    switch(c){
    case 's':                           /* seed for random number generator */
      args->s = atoi(optarg);
      break;
    case 'i':                           /* nuber of iterations */
      args->i = atoi(optarg);
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


void printUsage(){
  printf("Usage: %s [-i <iterations> -s <seed>] [inputFiles]\n",progname());
  printf("Bootstrap site frequency specta\n");
  printf("Example: bootSfs -i 10000 foo.sfs\n");
  printf("Options:\n");
  printf("\t[-i <NUM> number of iterations; default: %d]\n", DEFAULT_I);
  printf("\t[-s <NUM> seed for random number generator; default: file|system]\n");
  printf("\t[-h print this help message and exit]\n");
  printf("\t[-v print program information and exit]\n");
  exit(0);
}

void printSplash(){
  printf("%s %s, %s\n",progname(), VERSION, DATE);
  printf("Written by Bernhard Haubold.\n");
  printf("Distributed under the GNU General Public License.\n");
  printf("Please send bug reports to haubold@evolbio.mpg.de\n");
  exit(0);
}
