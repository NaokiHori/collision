#include "global.h"
#include "init.h"


const char initfname[]={"init.bin"};

static double gen_random(const double min, const double max){
  return (max-min)*rand()/(1.*RAND_MAX)+min;
}

static int init_positions(double *xs, double *ys){
  FILE *fp=NULL;
  int n;
  if((fp=fopen(initfname, "rb"))==NULL){
    printf("init file open error\n");
    ERR_HNDL(__FILE__, __LINE__, __func__);
  }
  for(n=0;n<NP;n++){
    if(fread(&(xs[n]), sizeof(double), 1, fp)==0){
      printf("reached end-of-file (or some error)\n");
      ERR_HNDL(__FILE__, __LINE__, __func__);
    }
    if(fread(&(ys[n]), sizeof(double), 1, fp)==0){
      printf("reached end-of-file (or some error)\n");
      ERR_HNDL(__FILE__, __LINE__, __func__);
    }
  }
  fclose(fp);
  return 0;
}

static int init_velocities(double *us, double *vs){
  int n;
  for(n=0;n<NP;n++){
    us[n]=gen_random(-1., 1.);
    vs[n]=gen_random(-1., 1.);
  }
  return 0;
}

#if defined(GENP)
static int gen_positions(void){
  FILE *fp=NULL;
  int n, nn, cnt=0;
  double xs[NP], ys[NP];
  double xvec, yvec;
  double x, y, dist;
  srand(time(NULL));
  for(n=0;n<NP;n++){
regen:
#if defined(XWALL)
    x=gen_random(RP, LX-RP);
#else
    x=gen_random(0., LX   );
#endif
#if defined(YWALL)
    y=gen_random(RP, LY-RP);
#else
    y=gen_random(0., LY   );
#endif
    for(nn=0;nn<cnt;nn++){
#if defined(XWALL)
      xvec=     fabs(x-xs[nn]);
#else
      xvec=fmin(fabs(x-xs[nn]), fmin(fabs(x-xs[nn]+LX), fabs(x-xs[nn]-LX)));
#endif
#if defined(YWALL)
      yvec=     fabs(y-ys[nn]);
#else
      yvec=fmin(fabs(y-ys[nn]), fmin(fabs(y-ys[nn]+LY), fabs(y-ys[nn]-LY)));
#endif
      dist=sqrt(xvec*xvec+yvec*yvec)-2.*RP;
      if(dist<=0.){
        goto regen;
      }
    }
    xs[nn]=x;
    ys[nn]=y;
    cnt++;
  }
  if((fp=fopen(initfname, "wb"))==NULL){
    printf("init file open error\n");
    ERR_HNDL(__FILE__, __LINE__, __func__);
  }
  for(n=0;n<NP;n++){
    fwrite(xs+n, sizeof(double), 1, fp);
    fwrite(ys+n, sizeof(double), 1, fp);
    printf("%2d: %lf, %lf\n", n, xs[n], ys[n]);
  }
  fclose(fp);
  return 0;
}
#endif

int init(double *xs, double *ys, double *us, double *vs){
#if defined(GENP)
  gen_positions();
#endif
  init_positions(xs, ys);
  init_velocities(us, vs);
  return 0;
}

