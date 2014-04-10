HEADERS = matrix.h stat.h log.h
CC = mpicc

#Set LOG_ON=1 if logs are wanted, or clear it with 0
CFLAGS += -D LOG_ON=0

	
mpi_matrix_add: mpi_matrix_add.o matrix.o $(HEADERS)
	$(CC) mpi_matrix_add.o matrix.o -o mpi_matrix_add

matrix_test: matrix.o matrix_test.o $(HEADERS)
	$(CC) matrix_test.o matrix.o -o matrix_test

mpi_matrix_add.o: mpi_matrix_add.c $(HEADERS)
	$(CC) $(CFLAGS) -c mpi_matrix_add.c -o mpi_matrix_add.o
	
matrix.o: matrix.h matrix.c $(HEADERS)
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

matrix_test.o: matrix.o $(HEADERS)
	$(CC) $(CFLAGS) -c matrix_test.c -o matrix_test.o
	
 	
	
clean:
	rm *.o matrix_test mpi_matrix_add
	
	
