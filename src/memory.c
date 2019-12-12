#include "global.h"
#include "memory.h"


#define ALLOCATE(p, size){\
  if((p=malloc(size))==NULL){\
    ERR_HNDL(__FILE__, __LINE__, __func__);\
  }\
}

int allocate(double **xs, double **ys, double **us, double **vs){
  size_t size=sizeof(double)*NP;
  ALLOCATE(*xs, size);
  ALLOCATE(*ys, size);
  ALLOCATE(*us, size);
  ALLOCATE(*vs, size);
  return 0;
}

int deallocate(double *xs, double *ys, double *us, double *vs){
  free(xs);
  free(ys);
  free(us);
  free(vs);
  return 0;
}

#undef ALLOCATE

