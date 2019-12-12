#include "global.h"
#include "update.h"


#if !defined(XWALL) || !defined(YWALL)
static double consider_period(double var, double len){
  return fmin(fabs(var), fmin(fabs(var+len), fabs(var-len)));
}
#endif

static double get_dist(const double xvec, const double yvec){
#if !defined(XWALL)
  const double xvec=consider_period(xvec, LX);
#endif
#if !defined(YWALL)
  const double yvec=consider_period(yvec, LY);
#endif
  return sqrt(xvec*xvec+yvec*yvec);
}

static double get_col_time_pp(double dx, double dy, const double du, const double dv){
#if !defined(XWALL)
  dx=consider_period(dx, LX);
#endif
#if !defined(YWALL)
  dy=consider_period(dy, LY);
#endif
  const double a=du*du+dv*dv;
  const double b=2.*(dx*du+dy*dv);
  const double c=dx*dx+dy*dy-4.*RP*RP;
  double sol0=(-b-sqrt(b*b-4.*a*c))/(2.*a);
  double sol1=(-b+sqrt(b*b-4.*a*c))/(2.*a);
  if(sol0<0.){
    return sol1;
  }
  if(sol1<0.){
    return sol0;
  }
  return fmin(sol0, sol1);
}

static int update_col_time(const double local_col_time, double *col_time, int *flag0, int *flag1, const int n0, const int n1){
  if(local_col_time<*col_time){
    *col_time=local_col_time;
    *flag0=n0;
    *flag1=n1;
  }
  return 0;
}

static int modpp(double *u0, double *v0, double *u1, double *v1, const double nx, const double ny){
  double ug, vg;
  double prod;
  ug=0.5*(*u0+*u1);
  vg=0.5*(*v0+*v1);
  prod=(*u0-ug)*nx+(*v0-vg)*ny;
  *u0-=2.*prod*nx;
  *v0-=2.*prod*ny;
  *u1+=2.*prod*nx;
  *v1+=2.*prod*ny;
  *u0*=EPARS;
  *v0*=EPARS;
  *u1*=EPARS;
  *v1*=EPARS;
  return 0;
}

static int mod_period(double *xs, double *ys){
#if !defined(XWALL)
  for(int n=0;n<NP;n++){
    if(xs[n]<0.){
      xs[n]+=LX;
    }else if(xs[n]>LX){
      xs[n]-=LX;
    }
  }
#endif
#if !defined(YWALL)
  for(int n=0;n<NP;n++){
    if(ys[n]<0.){
      ys[n]+=LY;
    }else if(ys[n]>LY){
      ys[n]-=LY;
    }
  }
#endif
  return 0;
}

int update(const int depth, double remain, double *xs, double *ys, double *us, double *vs){
  int flag0=-1, flag1=-1;
  double x0a, y0a, x1a, y1a; /* after  (t+dt) */
  double x0b, y0b, x1b, y1b; /* before (t   ) */
  double u0, v0, u1, v1;
  double dist;
  int n0, n1;
  double nx, ny, norm;
  double col_time=2.*remain;
  double local_col_time;
  if(depth>MAXDEPTH){
    printf("too deep recursive\n");
    ERR_HNDL(__FILE__, __LINE__, __func__);
  }
  for(n0=0;n0<NP;n0++){
    x0b=xs[n0];
    y0b=ys[n0];
    u0=us[n0];
    v0=vs[n0];
    x0a=x0b+u0*remain;
    y0a=y0b+v0*remain;
    for(n1=n0+1;n1<NP;n1++){
      local_col_time=2.*remain;
      x1b=xs[n1];
      y1b=ys[n1];
      u1=us[n1];
      v1=vs[n1];
      x1a=x1b+u1*remain;
      y1a=y1b+v1*remain;
      dist=get_dist(x1a-x0a, y1a-y0a);
      if(dist<2.*RP){
        local_col_time=get_col_time_pp(x1b-x0b, y1b-y0b, u1-u0, v1-v0);
        update_col_time(local_col_time, &col_time, &flag0, &flag1, n0, n1);
      }
    }
  }
  for(n0=0;n0<NP;n0++){
    x0b=xs[n0];
    y0b=ys[n0];
    u0=us[n0];
    v0=vs[n0];
    x0a=x0b+u0*remain;
    y0a=y0b+v0*remain;
#if defined(XWALL)
    /* x-minus wall */
    dist=x0a-RP;
    if(dist<=0.){
      local_col_time=-(x0b-RP)/u0;
      update_col_time(local_col_time, &col_time, &flag0, &flag1, n0, NP);
    }
    /* x-plus wall */
    dist=LX-x0a-RP;
    if(dist<=0.){
      local_col_time=(LX-x0b-RP)/u0;
      update_col_time(local_col_time, &col_time, &flag0, &flag1, n0, NP+1);
    }
#endif
#if defined(YWALL)
    /* y-minus wall */
    dist=y0a-RP;
    if(dist<=0.){
      local_col_time=-(y0b-RP)/v0;
      update_col_time(local_col_time, &col_time, &flag0, &flag1, n0, NP+2);
    }
    /* y-plus wall */
    dist=LY-y0a-RP;
    if(dist<=0.){
      local_col_time=(LY-y0b-RP)/v0;
      update_col_time(local_col_time, &col_time, &flag0, &flag1, n0, NP+3);
    }
#endif
  }
  if(col_time>remain){
    // no collision
    for(n0=0;n0<NP;n0++){
      xs[n0]+=us[n0]*remain;
      ys[n0]+=vs[n0]*remain;
    }
    // periodicity
    mod_period(xs, ys);
  }else{
    // collision
    for(n0=0;n0<NP;n0++){
      xs[n0]+=us[n0]*col_time;
      ys[n0]+=vs[n0]*col_time;
    }
    // modify velocity
    if(flag1==NP || flag1==NP+1){
      // x-wall collision
#if defined(XWALL)
      us[flag0]*=-1.*EWALL;
#endif
    }else if(flag1==NP+2 || flag1==NP+3){
      // y-wall collision
#if defined(YWALL)
      vs[flag0]*=-1.*EWALL;
#endif
    }else{
      // particle collision
      nx=xs[flag1]-xs[flag0];
      ny=ys[flag1]-ys[flag0];
#if !defined(XWALL)
      nx=consider_period(nx, LX);
#endif
#if !defined(YWALL)
      ny=consider_period(ny, LY);
#endif
      norm=sqrt(nx*nx+ny*ny);
      nx/=norm;
      ny/=norm;
      modpp(&(us[flag0]), &(vs[flag0]), &(us[flag1]), &(vs[flag1]), nx, ny);
    }
    // periodicity
    mod_period(xs, ys);
    update(depth+1, remain-col_time, xs, ys, us, vs);
  }
  return 0;
}

