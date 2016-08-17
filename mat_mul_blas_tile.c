#include <stdio.h>              /* I/O lib         ISOC     */
#include <stdlib.h>             /* Standard Lib    ISOC     */
#include <cblas.h>              /* Basic Linear Algebra I/O */
#include <sys/time.h>

#define M 2048
#define K 2048
#define N 1
#define TILE_M 16
#define TILE_K 16
#define TILE_N 1

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
        for (j=0; j<K; j++) {
            A[i*K + j] = i+j+1;
        }
    }
    for (i=0; i<K; i++) {
        for (j=0; j<N; j++) {
            B[i*N + j] = i+j+1;            
        }
    }
}

int main(int argc, char **argv) {

 //  printf("inside main\n");
   double *a_matrix, *b_vector, *c_vector, *a_tile, *b_tile, *c_tile;
   double t_start, t_end, alpha, beta;
   int i,j,k,m,n,k_tile,lda,ldb,ldc,begin_value;
   a_matrix = (double *) malloc( M*K*sizeof( double ));
   b_vector = (double *) malloc( K*N*sizeof( double ));
   c_vector = (double *) malloc( M*N*sizeof( double ));
   init_array(a_matrix,b_vector);
  // printMatrix(M,K,a,"A");
  // printMatrix(K,N,b,"B");

   IF_TIME(t_start = rtclock());
             /* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
       for(i=0; i<M; i+=TILE_M)
         for(j=0; j<N; j+=TILE_N)
           for(k=0; k<K; k+=TILE_K)
           {
              alpha = 1.0;
              beta = 0.0;
              lda = K;
              ldb = N;
              ldc = N;
              m = TILE_M;
              n = TILE_N;
              k_tile = TILE_K;
              a_tile = &a_matrix[i*K+k];
              b_tile = &b_vector[k*N+j];
              c_tile = &c_vector[i*N+j];
              cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,m,n,k_tile,alpha,a_tile,lda,b_tile,ldb,beta,c_tile,ldc);
             // cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M,    M,    N, 1.0,   a,   N, b, M,  0.0, c,  M);
           }
//   printMatrix(M,M,c,"C");
   IF_TIME(t_end = rtclock());
   IF_TIME(fprintf(stderr, "%0.6lfs\n", t_end - t_start));

  // printf("Done\n");
   free(a_matrix);
   free(b_vector);
   free(c_vector);
   return 0;
}
