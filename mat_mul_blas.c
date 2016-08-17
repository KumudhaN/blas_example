#include <stdio.h>              /* I/O lib         ISOC     */
#include <stdlib.h>             /* Standard Lib    ISOC     */
#include <cblas.h>              /* Basic Linear Algebra I/O */
#include <sys/time.h>

#define M 1024
#define N 1024

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

void printMatrix(int m, int n, double *a, const char* str)
{
   int i,j;
   printf("%s\n",str);
   for(i =0;i<m;i++)
   {
       for(j=0;j<n;j++)
          printf("%lf  ",a[i*n+j]);
       printf("\n");
   }
}

void init_array(double* A, double* B)
{
    int i, j;

   printf("inside init array\n");
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
            A[i*N + j] = (i + j);
            B[i*N + j] = (double)(i*j);            
        }
    }
}

int main(int argc, char **argv) {

   printf("inside main\n");
   double *a, *b, *c;
   double t_start, t_end;
   a = (double *) malloc( M*N*sizeof( double ));
   b = (double *) malloc( N*M*sizeof( double ));
   c = (double *) malloc( M*M*sizeof( double ));
   init_array(a,b);

   IF_TIME(t_start = rtclock());
             /* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M,    M,    N, 1.0,   a,   N, b, M,  0.0, c,  M);
   IF_TIME(t_end = rtclock());
   IF_TIME(fprintf(stderr, "%0.6lfs\n", t_end - t_start));

   printf("Done\n");
   free(a);
   free(b);
   free(c);
   return 0;
}
