#include <stdio.h>              /* I/O lib         ISOC     */
#include <stdlib.h>             /* Standard Lib    ISOC     */
#include <cblas.h>              /* Basic Linear Algebra I/O */
#include <sys/time.h>

#define M 1024
#define N 1
#define K 1024

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

void init_array(double* A, double* B, double *C)
{
    int i, j;

   printf("inside init array\n");
    for (i=0; i<M; i++) {
        for (j=0; j<K; j++) {
            A[i*N + j] = i + j + 1;
        }
    }
    for (i=0; i<K; i++) {
        for (j=0; j<N; j++) {
            B[i*N + j] = i+j+1;            
        }
    }
    for (i=0; i<K; i++) {
        for (j=0; j<N; j++) {
            C[i*N + j] = i+j+1;            
        }
    }
}

int main(int argc, char **argv) {

   printf("inside main\n");
   double *a, *b, *c, *d, *e;
   double t_start, t_end;
   a = (double *) malloc( M*K*sizeof( double ));
   b = (double *) malloc( K*N*sizeof( double ));
   c = (double *) malloc( M*N*sizeof( double ));
   d = (double *) malloc( K*N*sizeof( double ));
   e = (double *) malloc( M*N*sizeof( double ));
   init_array(a,b,d);

   IF_TIME(t_start = rtclock());
             /* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M,    N,    K, 1.0,   a,   K, b, N,  0.0, c,  N);
   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M,    N,    K, 1.0,   a,   K, d, N,  0.0, e,  N);
   IF_TIME(t_end = rtclock());
   IF_TIME(fprintf(stderr, "%0.6lfs\n", t_end - t_start));

   printf("Done\n");
   free(a);
   free(b);
   free(c);
   return 0;
}
