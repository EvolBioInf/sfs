/***** sfs.c **************************************
 * Description: Manipulate site frequency spectra.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Nov 28 12:00:10 2018
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "tab.h"
#include "eprintf.h"
#include "gsl_rng.h"
#include "sfs.h"

void freeSfs(Sfs *s) {
  free(s->G);
  free(s->a);
  free(s);
}

void iniBoot(Sfs *os) {
  int k = 0;
  os->a = (int *)emalloc(os->s * sizeof(int));
  for(int i = 0; i < os->n; i++)
    for(int j = 0; j < os->G[i]; j++)
      os->a[k++]  = i;
  if(k != os->s) {
    fprintf(stderr, "ERROR[bootSfs]: iniBoot.\n");
    exit(-1);
  }
}

void printSfs(Sfs *s) {
  printf("#r\tf(r)\n");
  for(int i = 0; i < s->n; i++)
    printf("%d\t%d\n", i, s->G[i]);
}

void bootSfs(Sfs *os, Sfs *bs, gsl_rng *r) {
  if(os->a == NULL)
    iniBoot(os);
  for(int i = 0; i < os->n; i++)
    bs->G[i] = 0;
  for(int i = 0; i < os->s; i++){
    int y = (int)(gsl_rng_uniform(r) * os->s);
    bs->G[os->a[y]]++;
  }
}

Sfs *newSfs(int n) {
  Sfs *sfs = (Sfs *)emalloc(sizeof(Sfs));
  sfs->n = n;
  sfs->G = (int *)emalloc(n * sizeof(int));
  sfs->a = NULL;
  for(int i = 0; i < n; i++)
    sfs->G[i] = 0;
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
    sfs->n = atoi(tabField(0));  /* degree */
    f      = atoi(tabField(1));  /* count  */
    sfs->G = (int *)erealloc(sfs->G, (sfs->n + 1) * sizeof(int));
    sfs->G[sfs->n] = f;
    sfs->s += sfs->G[sfs->n];
  }
  sfs->n++;
  if(line == NULL)
    last = 1;

  return sfs;
}
