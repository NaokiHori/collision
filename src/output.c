#include "global.h"
#include "output.h"


int output(const int step, const double *xs, const double *ys, const double *us, const double *vs){
  FILE *fp=NULL;
  char fname[128];
  double energy=0.;
  int n;
  sprintf(fname, "energy.dat");
  if(step==0){
    fp=fopen(fname, "w");
  }else{
    fp=fopen(fname, "a");
  }
  for(n=0;n<NP;n++){
    energy+=us[n]*us[n]+vs[n]*vs[n];
  }
  fprintf(fp, "%10d %.15f\n", step, energy);
  fclose(fp);
  sprintf(fname, "positions.dat");
  if(step==0){
    fp=fopen(fname, "w");
  }else{
    fp=fopen(fname, "a");
  }
  for(n=0;n<NP;n++){
    fprintf(fp, "%.15f %.15f ", xs[n], ys[n]);
  }
  fprintf(fp, "\n");
  fclose(fp);
  return 0;
}

