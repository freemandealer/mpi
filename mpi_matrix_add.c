#include <stdio.h>
#include "matrix.h"
#include "log.h"
#include "stat.h"
#include "mpi.h"

#define TAG_MATRIX1 99
#define TAG_MATRIX2 98
#define TAG_RESULT  97


int main(int argc, char *argv[])
{
	int ret;
	int myid;
	int pid;           // Sub-matrix is send to the process
	int size;
	double buf[2048];
	int row_id;      // identify the index of a sub matrix
	MPI_Status status;
	matrix_t *m_matrip1, *m_matrip2, *result_matrip;
	double *sub_array1, *sub_array2, *result_array;
	int m_row, m_col;  // tow matrixes share the same size
	int i,j;

	/* Create matrixes */
	printf("How many rows:\n"); scanf("%d", &m_row);
	printf("How many columns:\n"); scanf("%d", &m_col);
	m_matrip1 = init_matrix(m_row, m_col);
	m_matrip2 = init_matrix(m_row, m_col);
	result_matrip = init_matrix(m_row, m_col);
	if (m_matrip1 == NULL || m_matrip2 == NULL || result_matrip == NULL) {
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
	
	if (myid == 0) {
		/* Dispatch matrixes */
		for (row_id=0; row_id<m_row; row_id++) {
			
			/* It's a little bit tricky. We distribute each row of the
		 	 * two matrixes into different node(with a Mod operation for
		 	 * we may not have enough nodes). We copy data into the
		 	 * sending buffer and put the row id at the end of the
		 	 * buffer so the node can tell which row it is. We use 
		 	 * different message TAGs to identify the tow matrixes.
		 	 */
			pid = row_id%size;
			memcpy(buf, m_matrix1->table[row_id], m_col*sizeof(double));
			buf[m_col] = (double)row_id;
			/* Add 1 to length coz we append row id at the tail*/
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX1,
						 MPI_COMM_WORLD);       
			memcpy(buf, m_matrix2->table[row_id], m_col*sizeof(double));
			buf[m_col] = (double)row_id;
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX2,
						 MPI_COMM_WORLD);
			/* Print out buf to check if everything is going right */
			for (j=0; j<(m_col+1); j++) {
				printf("Buffer content:\n");
				printf("%lf ", buf[j]);
			}
		}
		/* Receive results */
		MPI_Recv(buf, m_col+1, MPI_DOUBLE, 0/*FIXME*/, TAG_RESULT, MPI_COMM_WORLD); 
		memcpy(result_matrip->table[(int)buf[m_col]], buf, 
				m_col*sizeof(double));
		/* Done, print the result!*/ //TODO
		print_matrix(result_matrip)
		
	} else {
		MPI_Recv(buf, m_col+1, MPI_DOUBLE, 0, TAG_MATRIX1, MPI_COMM_WORLD);
		row_id = (int)buf[m_col]; 			// Get row id
		DLOG("Recved row id:%d\n",row_id);
		memcpy(sub_array1, buf,m_col*sizeof(double));
		MPI_Recv(buf, m_col+1, MPI_DOUBLE, 0, TAG_MATRIX1, MPI_COMM_WORLD);
		
		/* Compare row id. I'm not sure whether the two sub-matrixex share 
		 * the same row id(same line)
		 */
		if (row_id != (int)buf[m_col]) {
			ELOG("Row ids are not same.\n");
			return -1;
		}
		memcpy(sub_array2, buf,m_col*sizeof(double));
		/* Calculate the addition */
		for (i=0; i<m_col; i++) {
			result_array[i] = sub_array1[i] + sub_array2[i];
		}
		/* Append row id at the end of result_array*/
		result_array[m_col] = (double)row_id;
		/* Send the result back to main node */
		MPI_Send(buf, m_col+1, MPI_DOUBLE, 0, TAG_RESULT,
						 MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}

