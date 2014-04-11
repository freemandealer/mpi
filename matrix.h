/*
 * This source file include the defination of matrix_t
 * structure and its operations.
 */

#ifndef MATRIX_H_H
#define MATRIX_H_H

typedef struct _matrix {
	double **table;
	int row;
	int col;
} matrix_t;

matrix_t* init_matrix(int rownum, int colnum);
int set_matrix_pos_value(matrix_t *matrixp, int row, int col, 
							const double val);
int get_matrix_pos_value(matrix_t *matrixp, int row, int col, 
							double *valp);
/* 
 * It the return-version of get_matrix_pos_val(). But be careful when
 * you use this function coz it won't tall you whether your access is 
 * out of boundary. If you're not sure, please use get_matrix_pos_val()
 * instead.
 */
double get_matrix_pos_value_ret(matrix_t *matrix, int row, int col);
matrix_t* matrix_from_file(char *path);
void enter_matrix(matrix_t *matrip);
void print_matrix(const matrix_t *matrip);
#endif
