/***** sfs.h **************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Nov 28 12:00:12 2018
 **************************************************/
#ifndef SFS
#define SFS

/* site frequency spectrum */
typedef struct sfs {
  int          *G; /* the spectrum                         */
  int           n; /* size of G                            */
  double       *p; /* array of probabilities               */
  unsigned int *m; /* array of multinomial random variates */
  int           s; /* number of sites in sectrum           */
} Sfs;


Sfs *nextSfs(FILE *fp);
void bootSfs(Sfs *os, Sfs *bs, gsl_rng *r);
void printSfs(Sfs *s);
Sfs *newSfs(int n);
void freeSfs(Sfs *s);

#endif
