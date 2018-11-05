#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

double* gen_matrix(int n, int m);
int mmult(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols);
void compare_matrix(double *a, double *b, int nRows, int nCols);

/** 
    Program to multiply a matrix times a matrix using both
    mpi to distribute the computation among nodes and omp
    to distribute the computation among threads.
*/

int main(int argc, char* argv[])
{
  int nrows, ncols;
  double *aa;	/* the A matrix */
  double *bb;	/* the B matrix */
  double *cc1;	/* A x B computed using the omp-mpi code you write */
  double *cc2;	/* A x B computed using the conventional algorithm */
 
  //changes
  int aRow, aColumn, bColumn;
  int numberSent, sender;
  //

  int myid, numprocs;
  double starttime, endtime;
  MPI_Status status;
  
  /* insert other global variables here */ //changes
  double *buffer;
  int anstype, current_row;
  // for accumulating the sum
  double *result;
  //
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
  if (argc > 1) {
    
    //changes  
    aRow = atoi(argv[1]);
    aColumn = atoi(argv[2]);
    bColumn = atoi(argv[3]);  
      
    //nrows = atoi(argv[1]);
    //ncols = nrows;
    
    printf("The matrices' dimensions are %d x %d and %d x %d.\n", aRow, aColumn, aColumn, bColumn);

     // testing if the matrices are generated correctly
    aa = gen_matrix(a_row, a_col);
    bb = gen_matrix(a_col, b_col);

    printf("Matrix A:\n");
    for (int i = 0; i < a_row; i++) {
      for (int j = 0; j < a_col; j++) {
        printf("%.1f ", aa[i * a_col + j]);
      }
      printf("\n");
    }

    printf("Matrix B:\n");
    for (int i = 0; i < a_col; i++) {
      for (int j = 0; j < b_col; j++) {
        printf("%.1f ", bb[i * b_col + j]);
      }
      printf("\n");
    }
    
    nrows = aRow;
    ncols = bColumn;
    buffer = (double *) malloc(sizeof(double) * aColumn);
    
    //end changes
    
    if (myid == 0) {
      // Master Code goes here
        
      //changes
      //aa = gen_matrix(nrows, ncols);
      //bb = gen_matrix(ncols, nrows);
       
      cc1 = malloc(sizeof(double) * nrows * ncols);
      starttime = MPI_Wtime();
       
       // number of rows sent
      numberSent = 0;
        
       
      //cc1 = malloc(sizeof(double) * nrows * nrows); 
      //starttime = MPI_Wtime();
      
      
      /* Insert your master code here to store the product into cc1 */
      endtime = MPI_Wtime();
      printf("%f\n",(endtime - starttime));
      cc2  = malloc(sizeof(double) * nrows * nrows);
      mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
      compare_matrices(cc2, cc1, nrows, nrows);
      
    } else {
      // Slave Code goes here
    }
  } else {
    fprintf(stderr, "Usage matrix_times_vector <size>\n");
  }
  MPI_Finalize();
  return 0;
}
