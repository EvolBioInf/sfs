/***** sfs.c **************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 20 07:50:43 2017
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "eprintf.h"
#include "sfs.h"
#include "tab.h"

void scanFile(FILE *fp, Args *args){ 
  Sfs *sfs;

  sfs = newSfs(0, args->f);
  while((sfs = nextSfs(fp, sfs)) != NULL)
    printSfs(sfs);
}

int main(int argc, char *argv[]){
  int i;
  char *version;
  Args *args;
  FILE *fp;

  version = "0.1";
  setprogname2("ms2sfs");
  args = getArgs(argc, argv);
  if(args->v)
    printSplash(version);
  if(args->h || args->e)
    printUsage(version);
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
  tabFree();
  free(args);
  free(progname());
  return 0;
}
