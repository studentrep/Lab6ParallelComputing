#include "mpi.h"
#include <stdio.h>
#include<math.h>
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
  int myid, numprocs;
  double starttime, endtime;
  
  MPI_Status status;
  /* insert other global variables here */
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
  //code from canvas to generate a random matrix
  if(argc>3)
  {
      srand(time(NULL));
      int numRow=atoi(argv[1]); //number of rows that the random matrix should have
      int numCol=atoi(argv[2]); //number of columns that the random matrix should have
      FILE * fp; //pointer to a file (will contain the new generated matrix)
      fp=fopen(argv[3],"w"); //creates the file
      fprintf(fp,"rows(%d) cols(%d)\n",numRow,numCol); //writes the size of matrix to the first line of the file 
      int i,j; 
      //the for loops generate the random numbers that make up the new matrix's entries
      for(i=0;i<numRow;i++)
      {
          for(j=0;j<numCol;j++)
          {
              fprintf(fp,"%f ",(double)rand()/RAND_MAX);
          }
      fprintf(fp,"\n"); //print a new line
      }
      fclose(fp); //close the new matrix file
  }
  
  // end of code from canvas
  
  if (argc > 1) {
    nrows = atoi(argv[1]);
    ncols = nrows;
    if (myid == 0) {
      // Master Code goes here
      aa = gen_matrix(nrows, ncols);
      bb = gen_matrix(ncols, nrows);
      cc1 = malloc(sizeof(double) * nrows * nrows); 
      starttime = MPI_Wtime();
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