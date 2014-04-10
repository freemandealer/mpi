#include <stdio.h>
#include "matrix.h"
#include "log.h"
#include "stat.h"
#include "mpi.h"

int main(int argc, char *argv[])
{
	int ret;
	int myid;
	int size;
	char buf[2048];
	MPI_Status status;
	matrix_t* m_matrip1, m_matrip2;
	int m_row, m_col;  // tow matrixes share the same size
	int i,j;

	/* Create matrixes */
	printf("How many rows:\n"); scanf("%d", &m_row);
	printf("How many columns:\n"); scanf("%d", &m_col);
	matrip1 = init_matrix(m_row, m_col);
	matrip2 = init_matrix(m_row, m_col);
	printf("Enter the matrix1:\n");
	for (i=0; i<m_row; i++) {
		for (j=0; j<m_col; j++)
		scanf("%d", &matrip1[i][j]);
	}
	printf("Enter the matrix1:\n");
	for (i=0; i<m_row; i++) {
		for (j=0; j<m_col; j++)
		scanf("%d", &matrip1[i][j]);
	}
	
	set_matrix_pos_value(matrixp, 2, 1, 3);
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Divide matrix */
	
	MPI_Finalize();
	return 0;
}

