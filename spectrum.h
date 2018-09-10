/***** spectrum.h *********************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 20 23:51:32 2017
 **************************************************/
#ifndef SPECTRUM
#define SPECTRUM

#include "sample.h"

/* define container for site frequency spectrum */
typedef struct spectrum{
  char isFolded;    /* spectrum is folded */
  int n;            /* sample size */
  int nFolded;      /* folded sample size */
  double *spectrum; /* the actual site frequency spectrum */
} Spectrum;

Spectrum *computeSpectrum(Sample *s, Spectrum *sp);
void printSpectrum(Spectrum *s);
void freeSectrum(Spectrum *sp);
void resetSpectrum(Spectrum *sp);
Spectrum *newSpectrum(int n);
void foldSpectrum(Spectrum *sp);
Spectrum *getArtificialSpectrum(Args *args);

#endif
