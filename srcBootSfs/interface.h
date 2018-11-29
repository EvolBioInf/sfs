#ifndef INTERFACE
#define INTERFACE

#define DEFAULT_I 10000

/* define argument container */
typedef struct args{
  char h;   /* help message? */
  char v;   /* version message? */
  char e;   /* error message? */
  int  i;   /* number of iterations */
  int  s;   /* seed for random number generator */
  char **inputFiles;
  int numInputFiles;
} Args;

Args *getArgs(int argc, char *argv[]);
void printUsage(char *version);
void printSplash(char *version);

#endif
