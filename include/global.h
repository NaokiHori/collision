#if !defined(GLOBAL_H)
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DIM 2
#define NP 25
#define XWALL
#define YWALL
#define LX 8.
#define LY 6.
#define RP 0.5
#define MAXDEPTH 100

#define EWALL 1.00
#define EPARS 1.00

#define GENP

#define ERR_HNDL(FILENAME, LINENUM, FUNCNAME) {                                               \
  printf("error detected in %s line %d (function: %s)\n", (FILENAME), (LINENUM), (FUNCNAME)); \
  exit(EXIT_FAILURE);                                                                         \
}

#endif // GLOBAL_H
