/***** sample.h **************************************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Tue Aug 14 20:35:28 2007.
 * License: GNU General Public
 *****************************************************************************/
#ifndef SAMPLE
#define SAMPLE

#include <stdio.h>
#include "interface.h"

typedef struct sample{
  int howmany;          /* number of samples to be generated */
  int nsam;             /* sample size */
  int nsite;            /* number of recombining sites */
  int maxlen;           /* maximum length of a haplotype */
  int segsites;         /* number of segregating sites */
  int npop;             /* number of populations */
  int *sampleSizes;     /* individuals sampled per population */
  int *map;             /* map positions of segregating sites onto sequence positions */
  float *positions;     /* positions of segregating sites */
  char **haplotypes;    /* raw haplotype data */
  char **seq;           /* ancestral sequence & its mutated version */
  char *line;           /* line buffer */
}Sample;

Sample *initializeSample(FILE *fp, Args *args);
Sample *getSample(int isMacs);
void outputSample(FILE *fpo); 
void freeSample();
void printFasta(FILE *fpo);
void printIma(FILE *fpo);
void printGenetree(FILE *fpo);

#endif
