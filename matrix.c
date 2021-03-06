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
	DLOG("Matrix alloc successful:\n matrix_t addr:%p\n table addr:%p\n",
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

matrix_t* matrix_from_file(char *path)
{
	FILE *filp;
	int row_num, col_num;
	matrix_t *matrip;
	int i,j;
	
	filp = fopen(path, "r");
	if (!filp) {
		ELOG("Failed to open data file.\n");
		return NULL;
	}
	fscanf(filp, "%d %d", &row_num, &col_num);
	matrip = init_matrix(row_num, col_num);
	if (!matrip) {
		ELOG("Can't create matrix from date file.\n");
		return NULL;
	}
	for (i=0; i<matrip->row; i++) {
		for (j=0; j<matrip->col; j++) {
			fscanf(filp,"%lf", &matrip->table[i][j]);
		}
	}
	DLOG("Matrix is created from data file.\n");
	fclose(filp);
	return matrip;
}

void enter_matrix(matrix_t *matrip)
{
	int i,j;
	
	if (!matrip) {
		ELOG("Matrix null pointer.\n");
		return;
	}
	for (i=0; i<matrip->row; i++) {
		for (j=0; j<matrip->col; j++) {
			scanf("%lf", &matrip->table[i][j]);
		}
	}
	DLOG("enter_matrix exit normally.\n");
}

void print_matrix(const matrix_t *matrip)
{
	int i,j;
	
	if (!matrip) {
		ELOG("Matrix null pointer.\n");
		return;
	}
	for (i=0; i<matrip->row; i++) {
		for (j=0; j<matrip->col; j++) {
			printf("%lf ", matrip->table[i][j]);
		}
		printf("\n");
	}
	DLOG("print_matrix exit normally.\n");
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
 * you use this function coz it won't tell you whether your access is 
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

