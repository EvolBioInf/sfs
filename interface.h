#ifndef INTERFACE
#define INTERFACE

#define DEFAULT_B 32 * BUFSIZ /* default buffer size */

/* define argument container */
typedef struct args{
  char h;   /* help message? */
  char v;   /* version message? */
  char e;   /* error message? */
  char a;   /* hidden option */
  char f;   /* fold spectrum? */
  char r;   /* raw counts? */
  char i;   /* inidvidual SFS? */
  double t; /* theta */
  int n;    /* sample size */
  int s;    /* hidden option */
  int m;    /* hidden option */
  char c;   /* hidden option */
  char **inputFiles;
  int b;    /* buffer size */
  int numInputFiles;
} Args;

Args *getArgs(int argc, char *argv[]);
void printUsage(char *version);
void printSplash(char *version);

#endif
