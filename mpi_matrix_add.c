#include <stdio.h>
#include "matrix.h"
#include "log.h"
#include "stat.h"
#include "mpi.h"

int main()
{
	int ret;
	matrix_t* matrixp;
	
	matrixp = init_matrix(3, 4);
	set_matrix_pos_value(matrixp, 2, 1, 3);
	printf("%lf\n", get_matrix_pos_value_ret(matrixp, 2, 1));
	return 0;
}

