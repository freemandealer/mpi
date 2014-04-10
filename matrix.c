/*
 * This source file include the defination of matrix_t
 * structure and its operations.
 */

#include "matrix.h"
#include "stat.h"
#include "log.h"
#include <stdio.h>
#include <malloc.h>

matrix_t* init_matrix(int rownum, int colnum)
{
	int i,j;
	matrix_t *matrix;
	
	matrix = (matrix_t *)malloc(sizeof(matrix_t));
	if (!matrix)
		goto matrix_alloc_fail;
	matrix->row = rownum;
	matrix->col = colnum;
	matrix->table = (double **)malloc(rownum * sizeof(double*));
	if (!matrix->table)
		goto matrix_table_alloc_fail;
	for (i=0; i<colnum; i++) {
		matrix->table[i] = (double *)malloc(colnum * sizeof(double));
		if (!matrix->table[i])
			goto matrix_row_alloc_fail;
	}
	DLOG("Matrix alloc successful:\n matrix_t addr:%p\n matrix.table addr:%p\n",
			matrix,matrix->table);
	return matrix;
	
matrix_row_alloc_fail:
	for (j=0; j<i; j++) {
		free(matrix->table[j]);
	}
	free(matrix->table);
	
matrix_table_alloc_fail:
	free(matrix);
	
matrix_alloc_fail:
	ELOG("Matrix alloc failed.\n");
	return NULL;
}

int set_matrix_pos_value(matrix_t *matrix, int row, int col, 
							const double val)
{
	if (!matrix) {
		ELOG("Matrix null pointer.\n");
		return STAT_ERROR;
	}
	if (row >= matrix->row || col >= matrix->col) {
		ELOG("Matrix access out of boundary.\n");
		return STAT_OUT_OF_BOUND;
	}
	matrix->table[row][col] = val;
	return STAT_OK;
}

int get_matrix_pos_value(matrix_t *matrix, int row, int col, 
							double *val)
{
	if (!matrix) {
		ELOG("Matrix null pointer.\n");
		return STAT_ERROR;
	}
	if (row >= matrix->row || col >= matrix->col) {
		ELOG("Matrix access out of boundary.\n");
		return STAT_OUT_OF_BOUND;
	}
	*val = matrix->table[row][col];
	return STAT_OK;
}

/* 
 * It the return-version of get_matrix_pos_val(). But be careful when
 * you use this function coz it won't tall you whether your access is 
 * out of boundary. If you're not sure, please use get_matrix_pos_val()
 * instead.
 */
double get_matrix_pos_value_ret(matrix_t *matrix, int row, int col)
{
	double val;
	
	if (get_matrix_pos_value(matrix, row, col, &val) == STAT_OK)
		return val;
	else
		return 0;
}

