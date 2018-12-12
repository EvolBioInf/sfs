/***** bootSfs.c **********************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Nov 28 11:58:11 2018
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "eprintf.h"
#include "gsl_rng.h"
#include "sfs.h"
#include "tab.h"

void scanFile(FILE *fp, Args *args, gsl_rng *r){
  Sfs *o = NULL, *b = NULL;
  while((o = nextSfs(fp)) != NULL) {
    b = newSfs(o->n);
    for(int i = 0; i < args->i; i++) {
      bootSfs(o, b, r);
      printSfs(b);
    }
    freeSfs(b);
    freeSfs(o);
  }
}

int main(int argc, char *argv[]){
  int i;
  Args *args;
  FILE *fp;
  gsl_rng *r;

  setprogname2("bootSfs");
  args = getArgs(argc, argv);
  r = ini_gsl_rng(args);
  if(args->v)
    printSplash();
  if(args->h || args->e)
    printUsage();
  if(args->numInputFiles == 0){
    fp = stdin;
    scanFile(fp, args, r);
  }else{
    for(i=0;i<args->numInputFiles;i++){
      fp = efopen(args->inputFiles[i],"r");
      scanFile(fp, args, r);
      fclose(fp);
    }
  }
  free_gsl_rng(r, args);
  free(args);
  free(progname());
  tabFree();
  return 0;
}

