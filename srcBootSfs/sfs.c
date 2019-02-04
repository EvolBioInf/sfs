/***** sfs.c **************************************
 * Description: Manipulate site frequency spectra.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Nov 28 12:00:10 2018
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_randist.h>
#include "tab.h"
#include "eprintf.h"
#include "gsl_rng.h"
#include "sfs.h"

void freeSfs(Sfs *s) {
  free(s->G);
  free(s->p);
  free(s->m);
  free(s);
}

void printSfs(Sfs *s) {
  printf("#r\tf(r)\n");
  for(int i = 0; i < s->n; i++)
    printf("%d\t%d\n", i, s->G[i]);
}

void bootSfs(Sfs *os, Sfs *bs, gsl_rng *r) {
  gsl_ran_multinomial(r, os->n, os->s, os->p, os->m);
  for(int i = 0; i < os->n; i++)
    bs->G[i] = os->m[i];
}

Sfs *newSfs(int n) {
  Sfs *sfs = (Sfs *)emalloc(sizeof(Sfs));
  sfs->n = n;
  sfs->G = (int *)emalloc(n * sizeof(int));
  sfs->m = (unsigned int *)emalloc(n * sizeof(unsigned int));
  sfs->p = (double *)emalloc(n * sizeof(double));
  for(int i = 0; i < n; i++) {
    sfs->G[i] = 0;
    sfs->p[i] = 0;
    sfs->m[i] = 0;
  }
  sfs->s = 0;

  return sfs;
}

/* nextSfs reads the next SFS from an open file 
 * returns NULL after the last SFS.
 */
Sfs *nextSfs(FILE *fp) {
  static short first = 1;
  static short last  = 0;
  char *line;
  double f;

  if(last)
    return NULL;
  Sfs *sfs = newSfs(1);
  while((line = tabGetLine(fp)) != NULL){
    if(line[0] == '#') {          /* header line */
      if(first) {
	first = 0;
	continue;
      } else
	return sfs;
    }
    sfs->n = atoi(tabField(0));      /* degree */
    f      = (int)atof(tabField(1)); /* count  */
    sfs->G = (int *)erealloc(sfs->G, (sfs->n + 1) * sizeof(int));
    sfs->G[sfs->n] = f;
    sfs->s += sfs->G[sfs->n];
  }
  sfs->n++;
  if(line == NULL)
    last = 1;
  /* compute probability distribution */
  sfs->p = (double *)erealloc(sfs->p, sfs->n * sizeof(double));
  sfs->m = (unsigned int *)erealloc(sfs->m, sfs->n * sizeof(unsigned int));
  for(int i = 0; i < sfs->n; i++)
    sfs->p[i] = (double)sfs->G[i] / sfs->s;
  return sfs;
}
