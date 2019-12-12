#if !defined(MEMORY_H)
#define MEMORY_H

extern int allocate(double **xs, double **ys, double **us, double **vs);
extern int deallocate(double *xs, double *ys, double *us, double *vs);

#endif // MEMORY_H
