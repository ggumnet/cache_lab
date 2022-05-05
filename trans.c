/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int blockRow, blockCol;
    int row, col;
    int t0, t1, t2,t3,t4,t5,t6,t7;

    if(M==32 && N==32){
        for(blockRow=0; blockRow<M; blockRow+=8){
            for(blockCol=0; blockCol<N; blockCol+=8){
                for(row = blockRow; row<blockRow+8; row++){
                    for(col = blockCol; col<blockCol+8; col++){
                        if(row!=col){
                            B[col][row] = A[row][col];
                        }
                        else{
                            t0 = A[row][col];
                        }
                    }
                    if(blockRow==blockCol){
                        B[row][row] = t0;
                    }
                }
            }
        }
    }
    else if(M==64 && N==64){
        for(blockRow=0; blockRow<M; blockRow+=8){
            for(blockCol=0; blockCol<N; blockCol+=8){
                for(row=0; row<8; row++){
                    t0 = A[blockRow+row][blockCol+0];
                    t1 = A[blockRow+row][blockCol+1];
                    t2 = A[blockRow+row][blockCol+2];
                    t3 = A[blockRow+row][blockCol+3];
                    t4 = A[blockRow+row][blockCol+4];
                    t5 = A[blockRow+row][blockCol+5];
                    t6 = A[blockRow+row][blockCol+6];
                    t7 = A[blockRow+row][blockCol+7];              

                    B[blockCol+0][blockRow+row] = t0;
                    B[blockCol+1][blockRow+row] = t1;
                    B[blockCol+2][blockRow+row] = t2;
                    B[blockCol+3][blockRow+row] = t3;
                    B[blockCol+4][blockRow+row] = t4;
                    B[blockCol+5][blockRow+row] = t5;
                    B[blockCol+6][blockRow+row] = t6;
                    B[blockCol+7][blockRow+row] = t7;
                }
            }
        }
    }
    return; 
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

