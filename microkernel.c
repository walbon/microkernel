#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <blis/blis.h>
#include <math.h>

#define M 8
#define K 8
#define N 8

#define rand_min -5
#define rand_max 5

int main(){

    //microkernel usage
    // \alpha * A_{mxk} * B_{kxn} + \beta * C_{mxn}
    float alpha = 1.0;
    float beta  = 0.0;
    float* A = (float *) malloc(M*K*sizeof(float));
    float* B = (float *) malloc(K*N*sizeof(float));
    float* C_blis = (float *) calloc(M*N, sizeof(float));
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
            C_blis[i] = 0;
    }

    //openblas usage -- sanity check
    bli_sgemm( BLIS_NO_TRANSPOSE,
               BLIS_NO_TRANSPOSE,
               M, N, K,
               &alpha,
               A, /*rsa*/ K, /*csa*/ 1,
               B, /*rsb*/ N, /*csb*/ 1,
               &beta,
               C_blis, /*rsc*/ N, /*csc*/ 1);


  int i, j, k;
    for (i=0; i<M; i++) {
        for (j=0; j<N; j++)
            C_naive[N*i+j] *= beta;

        for (k=0; k<K; k++) { 
            for (j=0; j<N; j++)
                C_naive[N*i+j] += alpha * A[M*i+j] * B[N*j+k];
        }
    }

  int erro = 0;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      if (abs(C_blis[i * M + j] - C_naive[i * M + j]) > 1e-6) {
        //printf("Error: [%d][%d] blis %f != %f naive\n", i, j, (float)C_blis[i*N + j], (float)C_naive[i*N+j], i, j);
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
   //Print C
    printf("C_blis:\n");
    for(int i=0; i<M; i++){
        printf("C[%d][]: ", i);
        for(int j=0; j<N; j++)
            printf("%.0f ", C_blis[i*M+j]);
        printf("\n");
    }
   if (erro) {
    printf("Validação da saída C falhou! %i vezes\n", erro);
    return 1;
   } else {
    printf("Validação da saída C bem-sucedida!\n");
   }
}
