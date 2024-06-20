#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <blis/blis.h>
#include <math.h>
#include <cblas.h>

#define M 512
#define K 1024
#define N 512

#define rand_min -5
#define rand_max 5

int main(){

    //microkernel usage
    // \alpha * A_{mxk} * B_{kxn} + \beta * C_{mxn}
    float alpha = 1.0;
    float beta  = 0.0;
    float* A = (float *) malloc(M*K*sizeof(float));
    float* B = (float *) malloc(K*N*sizeof(float));
    float* C_blas = (float *) calloc(M*N, sizeof(float));
    float* C_naive = (float *) calloc(M*N, sizeof(float));

    //data initialization
    srand(42);
    for(int i=0; i<M; i++){
        for (int j=0; j<K; j++){
            A[M*i+j] = (float) i+1;
        }
    }
    for(int i=0; i<K; i++){
        for (int j=0; j<N; j++){
            B[N*i+j] = (float) j+1;
        }
    }
    for(int i=0; i<M*N; i++){
            C_naive[i] = 0;
            C_blas[i] = 0;
    }

    //openblas usage
    bli_sgemm( BLIS_NO_TRANSPOSE,
               BLIS_NO_TRANSPOSE,
               M, N, K,
               &alpha,
               A, /*rsa*/ K, /*csa*/ 1,
               B, /*rsb*/ N, /*csb*/ 1,
               &beta,
               C_blas, /*rsc*/ N, /*csc*/ 1);

     // Perform matrix multiplication using OpenBLAS
    //cblas_sgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA, enum CBLAS_TRANSPOSE TransB, 
    //f77_int M, f77_int N, f77_int K, float alpha, const float *A, f77_int lda, const float *B, 
    //f77_int ldb, float beta, float *C, f77_int ldc)
    
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              M, N, K,
              alpha, A, K, B, N,
              beta, C_naive, N);
  int i, j, k;
/*
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++)
            C_naive[i*N+j] *= beta;

        for (k=0; k<K; k++) { 
            for (j=0; j<N; j++)
                C_naive[i*N+j] += alpha * A[i*K+k] * B[j*N+k];
        }
    }
*/
  int erro = 0;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      if (abs(C_blas[i * M + j] - C_naive[i * M + j]) > 1e-6) {
        printf("Error: [%d][%d] blis %f != %f blas\n", i, j, (float)C_blas[i*N + j], (float)C_naive[i*N+j], i, j);
        erro += 1;
      }
    }
  }

   /*
   //Print A
    printf("A:\n");
    for(int i=0; i<M/10; i++){
        printf("A[%d][]: ", i);
        for(int j=0; j<N/10; j++)
            printf("%.0f ", A[i*M+j]);
        printf("\n");
    }
    //Print B
    printf("\n");
    printf("B:   ");
    for(int i=0; i<M*N; i++){
        printf("%.0f ", B[i]);
    }
   */
  if (erro) {
    printf("Validação da saída C falhou! %i vezes\n", erro);
    return 1;
  } else {
    printf("Validação da saída C bem-sucedida!\n");
  }
 
}
