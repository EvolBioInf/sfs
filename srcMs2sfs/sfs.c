/***** sfs.c **************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Mon Nov 26 18:19:45 2018
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eprintf.h"
#include "tab.h"
#include "sfs.h"

void foldSfs(Sfs *s) {
  for(int i = 1; i < s->n / 2; i++)
    s->G[i] += s->G[s->n - i];
}

void printSfs(Sfs *s) {
  int n;

  printf("#r\tf[r]\n");
  if(s->f)
    n = s->n / 2;
  else
    n = s->n - 1;
  for(int i = 1; i <= n; i++)
    printf("%d\t%d\n", i, s->G[i]);
}

void freeSfs(Sfs *s) {
  free(s->G);
  free(s);
}

Sfs *newSfs(int n, short f) {
  Sfs *s = (Sfs *)emalloc(sizeof(Sfs));
  s->f = f;
  s->n = n;
  s->m = n;
  s->G = emalloc(n * sizeof(int));
  for(int i = 0; i < n; i++)
    s->G[i] = 0;

  return s;
}

void extractSfs(Sfs *sfs, int *a, int l) {
  if(sfs->m < sfs->n) {
    sfs->m = sfs->n;
    sfs->G = (int *)erealloc(sfs->G, sfs->m * sizeof(int));
  }
  for(int i = 1; i < sfs->n; i++)
    sfs->G[i] = 0;
  for(int i = 0; i < l; i++)
    sfs->G[a[i]]++;
  if(sfs->f)
    foldSfs(sfs);
}

int *newArray(int l) {
  int *a = (int *)emalloc(l * sizeof(int));
  for(int i = 0; i < l; i++)
    a[i] = 0;
  return a;
}

void scanLine(char *line, int l, int *a) {
  for(int i = 0; i < l; i++)
    if(line[i] == '1')
      a[i]++;
}

Sfs *nextSfs(FILE *fp, Sfs *sfs) {
  static short eof = 0;
  short open = 0;
  char *line = NULL;
  int ln = 0, n = 0, l = 0, *a = NULL;

  if(eof) {
    freeSfs(sfs);
    if(a)
      free(a);
    return NULL;
  }

  while((line = tabGetPlainLine(fp)) != NULL) {
    ln++;
    if((line[0] == '1' || line[0] == '0') && ln > 2) {
      if(!open) {
	open = 1;
	n = 0;
	l = strlen(line);
	if(a)
	  free(a);
	a = newArray(l);
      }
      scanLine(line , l, a);
      n++;
    } else if(open) {
      sfs->n = n;
      extractSfs(sfs, a, l);
      open = 0;
      free(a);
      return sfs;
    }
  }
  eof = 1;
  sfs->n = n;
  extractSfs(sfs, a, l);
  free(a);
  return sfs;
}
