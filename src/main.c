#include "global.h"
#include "memory.h"
#include "init.h"
#include "update.h"
#include "output.h"


int main(void){
  const int stepmax=10000;
  const double basedt=0.001;
  double *xs=NULL;
  double *ys=NULL;
  double *us=NULL;
  double *vs=NULL;
  int step;
  allocate(&xs, &ys, &us, &vs);
  init(xs, ys, us, vs);
  for(step=0;step<stepmax;step++){
    update(0, basedt, xs, ys, us, vs);
    if(step%100==0){
      output(step, xs, ys, us, vs);
    }
    if(step%1000==0){
      printf("step %d\n", step);
    }
  }
  deallocate(xs, ys, us, vs);
  return 0;
}

