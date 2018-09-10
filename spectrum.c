/***** spectrum.c *********************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 20 23:51:30 2017
 **************************************************/
#include <stdlib.h>
#include "spectrum.h"
#include "eprintf.h"

Spectrum *getArtificialSpectrum(Args *args){
  Spectrum *sfs;
  int i, n;

  sfs = newSpectrum(args->n);
  n = args->n;
  for(i=0;i<=n;i++)
    sfs->spectrum[i] = args->t / (double)i;

  return sfs;
}

void resetSpectrum(Spectrum *sp) {
  for(int i = 0; i <= sp->n; i++)
    sp->spectrum[i] = 0;
}

Spectrum *newSpectrum(int n){
  Spectrum *sp;
  int i;

  sp = (Spectrum *)emalloc(sizeof(Spectrum));
  sp->n = n;
  sp->spectrum = (double *)emalloc((sp->n+1)*sizeof(double));
  for(i=0;i<=sp->n;i++)
    sp->spectrum[i] = 0.;
  sp->isFolded = 0;

  return sp;
}

Spectrum *computeSpectrum(Sample *s, Spectrum *sp){
  int i, j, x;

  /* traverse haplotypes */
  for(j=0;j<s->segsites;j++){
    x = 0;
    for(i=0;i<s->nsam;i++)
      if(s->haplotypes[i][j] == '1')
	x++;
    sp->spectrum[x]++;
  }

  return sp;
}

void printSpectrum(Spectrum *sp){
  int i, n;
  if(sp->isFolded)
    n = sp->nFolded;
  else
    n = sp->n;
  printf("#r\tf(r)\n");
  for(i=1;i<n;i++)
    printf("%d\t%.3e\n",i,sp->spectrum[i]);
}

void freeSectrum(Spectrum *sp){
  free(sp->spectrum);
  free(sp);
}

void foldSpectrum(Spectrum *sp){
  int i, n;

  sp->isFolded = 1;
  n = sp->n;
  if(n-1 % 2 == 1)
     n--;
  for(i=1;i<=(n-1)/2;i++)
    sp->spectrum[i] += sp->spectrum[n-i];
  sp->nFolded = (n-1) /  2 + 1;
  if((n-1) % 2)
    sp->nFolded++;
}
