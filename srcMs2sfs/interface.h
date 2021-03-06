#ifndef INTERFACE
#define INTERFACE

/* define argument container */
typedef struct args{
  char h;   /* help message? */
  char v;   /* version message? */
  char e;   /* error message? */
  char f;   /* fold spectrum? */
  char **inputFiles;
  int numInputFiles;
} Args;

Args *getArgs(int argc, char *argv[]);
void printUsage();
void printSplash();

#endif
