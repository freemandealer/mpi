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
	m_matrip1 = matrix_from_file("./matrix1.dat");
	m_matrip2 = matrix_from_file("./matrix2.dat");
	if (m_matrip1 == NULL || m_matrip2 == NULL) {
		printf("Failed to read matrixes from file.\n");
		return -1;
	}
	m_row = m_matrip1->row;
	m_col = m_matrip1->col;
	result_matrip = init_matrix(m_row, m_col);
	if (result_matrip == NULL) {
		printf("Memory alloc error.\n");
		return -1;
	}
	/* Check if everything is going right
	print_matrix(m_matrip1);
	print_matrix(m_matrip2);*/

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
		 	/* FIXME:What if the number of processes is larger */
			pid = row_id % (size-1);	// Process 0 isn't involved  
			pid ++;					
			memcpy(buf, m_matrip1->table[row_id], m_col*sizeof(double));
			/* Add 1 to length coz we append row id at the tail*/
			buf[m_col] = (double)row_id;
			DLOG("P0:buf:%lf %lf %lf %lf %lf\n", 
				buf[0], buf[1], buf[2], buf[3], buf[4]);
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX1,
						 MPI_COMM_WORLD);
			DLOG("P0:send a message1 to %d.\n", pid);
			memcpy(buf, m_matrip2->table[row_id], m_col*sizeof(double));
			buf[m_col] = (double)row_id;
			MPI_Send(buf, m_col+1, MPI_DOUBLE, pid, TAG_MATRIX2,
						 MPI_COMM_WORLD);
			DLOG("P0:send a message2 to %d.\n", pid);
			/* Print out buf to check if everything is going right 
			for (j=0; j<(m_col+1); j++) {
				printf("Buffer content:\n");
				printf("%lf ", buf[j]);
			}
			printf("\n"); */
		}
		/* Receive results */
		for (i=0; i<m_row; i++) {
			MPI_Recv(buf, m_col+1, MPI_DOUBLE, MPI_ANY_SOURCE,
						TAG_RESULT,MPI_COMM_WORLD, &status); 
			memcpy(result_matrip->table[(int)buf[m_col]], buf, 
						m_col*sizeof(double));
		}
		/* Done, print the result*/
		print_matrix(result_matrip);
		
	} else {
		/* FIXME:Don't expect finish the job by recving just once */
		MPI_Recv(buf, m_col+1, MPI_DOUBLE, 0, TAG_MATRIX1,
					MPI_COMM_WORLD, &status);
		row_id = (int)buf[m_col]; 			// Get row id
		memcpy(sub_array1, buf,m_col*sizeof(double));
		DLOG("P%d:Recved sub array1 row id:%d.\n", myid, row_id);
		MPI_Recv(buf, m_col+1, MPI_DOUBLE, 0, TAG_MATRIX1,
					MPI_COMM_WORLD, &status);
		
		/* Compare row id. I'm not sure whether the two 
		 * sub-matrixex share the same row id(same line)
		 */
		if (row_id != (int)buf[m_col]) {
			ELOG("Row ids are not same.\n");
			return -1;
		}
		memcpy(sub_array2, buf,m_col*sizeof(double));
		DLOG("P%d:Recved sub array2 row id:%d.\n", myid, row_id);
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

