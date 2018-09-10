/***** sfs.c **************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 20 07:50:43 2017
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "eprintf.h"
#include "sample.h"
#include "spectrum.h"

void scanFile(FILE *fp, Args *args){ 
  Sample *sa;
  Spectrum *sp;
  int i, n;

  sa = initializeSample(fp, args);
  sp = newSpectrum(sa->nsam);
  n = 0;
  while((sa = getSample(0)) != NULL){
    sp = computeSpectrum(sa, sp);
    if(args->i) {
      if(args->f)
	foldSpectrum(sp);
      printSpectrum(sp);
      resetSpectrum(sp);
    }
    n++;
  }
  if(!args->i) {
    if(!args->r){
      for(i=0;i<sp->n;i++)
	sp->spectrum[i] /= (double)n;
    }
    if(args->f)
      foldSpectrum(sp);
    printSpectrum(sp);
  }
  freeSample();
}

int main(int argc, char *argv[]){
  int i;
  char *version;
  Args *args;
  FILE *fp;
  Spectrum *sp;

  version = "0.7";
  setprogname2("sfs");
  args = getArgs(argc, argv);
  if(args->v)
    printSplash(version);
  if(args->h || args->e)
    printUsage(version);
  if(args->t){
    sp = getArtificialSpectrum(args);
    if(args->f)
      foldSpectrum(sp);
    printSpectrum(sp);
    return 0;
  }
  if(args->numInputFiles == 0){
    fp = stdin;
    scanFile(fp, args);
  }else{
    for(i=0;i<args->numInputFiles;i++){
      fp = efopen(args->inputFiles[i],"r");
      scanFile(fp, args);
      fclose(fp);
    }
  }
  free(args);
  free(progname());
  return 0;
}
