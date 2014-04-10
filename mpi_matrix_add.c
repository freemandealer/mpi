#include <stdio.h>
#include "matrix.h"
#include "log.h"
#include "stat.h"
#include "mpi.h"

#define TAG_MATRIX1 98
#define TAG_MATRIX2 99


int main(int argc, char *argv[])
{
	int ret;
	int myid;
	int pid;           // Sub-matrix is send to the process
	int size;
	double buf[2048];
	MPI_Status status;
	matrix_t *m_matrip1, *m_matrip2;
	int m_row, m_col;  // tow matrixes share the same size
	int i,j;

	/* Create matrixes */
	printf("How many rows:\n"); scanf("%d", &m_row);
	printf("How many columns:\n"); scanf("%d", &m_col);
	m_matrip1 = init_matrix(m_row, m_col);
	m_matrip2 = init_matrix(m_row, m_col);
	if (m_matrip1 == NULL || m_matrip2 == NULL) {
		printf("Memory alloc error! Exiting ...\n");
		return -1;
	}
	printf("Enter the matrix1:\n");
	enter_matrix(m_matrip1);
	printf("Enter the matrix2:\n");
	enter_matrix(m_matrip2);
	printf("Now matrix1 is:\n");
	print_matrix(m_matrip1);
	printf("and matrix2 is:\n");
	print_matrix(m_matrip2);
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	/* Dispatch matrixes */	
	if (myid == 0) {
		/* It's a little bit tricky. We distribute eachi row of the tow matrixes into different node(with a Mod operation for we may not have enough nodes). We put */
		for (i=0; i<m_row; i++) {
			pid = i%size;
			memcpy(buf, m_matrix1->table[i], m_col*sizeof(double));
			buf[m_col] = i;
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX1,
						 MPI_COMM_WORLD);
			memcpy(buf, m_matrix2->table[i], m_col*sizeof(double));
			buf[m_col] = i;
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX2,
						 MPI_COMM_WORLD);
		}
	} else {
	
	}
	
	
	MPI_Finalize();
	return 0;
}

