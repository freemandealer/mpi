#include <stdio.h>
#include "matrix.h"

int main()
{
	int ret;
	matrix_t* matrip;
	
	matrip = matrix_from_file("./matrix1.dat");
	print_matrix(matrip);
	return 0;
}

