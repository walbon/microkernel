#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <blis/blis.h>
#include <math.h>

#define M 512
#define K 1024
#define N 512

#define rand_min -5
#define rand_max 5

int main(){

    //microkernel usage
    // \alpha * A_{mxk} * B_{kxn} + \beta * C_{mxn}
    float* A = (float *) malloc(M*K*sizeof(float));
    float* B = (float *) malloc(K*N*sizeof(float));
    float* C_blas = (float *) calloc(M*N, sizeof(float));
    float* C_naive = (float *) calloc(M*N, sizeof(float));

    //data initialization
    srand(42);
    for(int i=0; i<M; i++){
        for (int j=0; j<N; j++){
            A[M*i+j] = (float) i+1; //((float)rand() / RAND_MAX) * (rand_max-rand_min) + rand_min;
            B[N*i+j] = (float) j+1; //((float)rand() / RAND_MAX) * (rand_max-rand_min) + rand_min;
        }
    }

    // //Print A
    // printf("A:   ");
    // for(int i=0; i<M*N; i++){
    //     printf("%.0f ", A[i]);
    // }

    // //Print B
    // printf("\n");
    // printf("B:   ");
    // for(int i=0; i<M*N; i++){
    //     printf("%.0f ", B[i]);
    // }

    float alpha = 1.0;
    float beta  = 1.0;

    //microkernel usage
    // for (int i = 0; i < M; i++){
    //     for (int j = 0; j < N; i++){
    //         for (int k = 0; k < K; i++){
    //             C[]
    //         }
            
    //     }
        
    // }
    

    //openblas usage -- sanity check
	bli_sgemm(  BLIS_NO_TRANSPOSE, BLIS_NO_TRANSPOSE,
	            M, N, K,
                 &alpha,
                A, /*rsa*/ K, /*csa*/ 1,
                B, /*rsb*/ N, /*csb*/ 1,
	            &beta,
                C_blas, /*rsc*/ N, /*csc*/ 1);


    printf("\n");
    double acc = 0.;
    for(int i=0; i<M; i++){
        for(int j = 0; j<N; j++){
            //printf("%.0f ", C_blas[M*i+j]);
        }
        //printf("\n");
    }
    printf("\nFINISH\n");


}
