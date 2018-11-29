/***** sfs.h **************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Mon Nov 26 18:19:47 2018
 **************************************************/
#ifndef SFS
#define SFS

/* define argument container */
typedef struct sfs{
  int  n;  /* sample size                         */
  int  *G; /* the spectrum                        */
  int   m; /* maximum sample size (capacity of G) */
  short f; /* folded?                             */
} Sfs;

Sfs *newSfs(int n, short f);
Sfs *nextSfs(FILE *fp, Sfs *sfs);
void printSfs(Sfs *s);

#endif
