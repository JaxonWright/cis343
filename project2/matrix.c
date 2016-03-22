/**
 * CIS 343 Project 2 - Matrix
 * Jaxon Wright & Chad Teitsma
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matrix.h"

//gcc -Wall -std=c99 -o matrix *.c
//valgrind ./matrix

/****************************************************************************
 * Creates and returns a pointer to a matrix object with the specified		*
 * number of rows and columns. The "data" field is set to a dynamically 	*
 * created array of ints of size rows * columns.							*
 *																			*
 * If the value of rows or columns is zero or negative, return NULL.		*
 ***************************************************************************/
Matrix *create(int rows, int columns) {
	
	if (rows < 1 || columns < 1)
    	return NULL;
    
	Matrix *result = malloc (sizeof(Matrix));
	result->rows = rows;
	result->columns = columns;
	result->data =  (int *)malloc(sizeof(int)*(rows*columns));
	return result;
}

/****************************************************************************
 * Returns the matrix element at (row,column). Return INT_MIN (limits.h)	*
 * if either row and/or column is invalid. Row and column values start at 	*
 * zero. DO NOT modify the input matrix.									*
 ***************************************************************************/
int getValueAt(Matrix *m, int row, int column) {
	if(m->rows-1 < row || m->columns-1 < column || row <0 || column <0)
	    return INT_MIN;
	
    return m->data[(row*m->columns) + column];
}

/****************************************************************************
 * If the row and column values are valid, sets the matrix element at 		*
 * (row,column) with the parameter value. Row and column values start at	*
 * zero.																	*
 ***************************************************************************/
void setValueAt(Matrix *m, int row, int column, int value) {
	if(m->rows-1 < row || m->columns-1 < column || row <0 || column <0) {
	    printf("invalid values input\n");
	    return;
	} else {
		//printf("setValueAt (%d, %d): %d\n", row, column, value);
        m->data[(row*m->columns) + column] = value;
	}
}

/****************************************************************************
 * If the input matrices are compatible, then performs matrix addition and	*
 * returns a pointer to the result matrix.									*
 * Use create(), getValueAt(), and setValueAt() functions to implement this	*
 * function. If the input matrices are not compatible, return NULL.			*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *add(Matrix *m1, Matrix *m2) {

	//Not comaptible
	if(m1->rows != m2->rows || m1->columns != m2->columns)
		return NULL;
		
	Matrix *result = create(m1->rows,m1->columns);
	//calculate values
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->columns; j++) {
			setValueAt(result,i,j,getValueAt(m1,i,j)+getValueAt(m2,i,j));
		}
	}

	return result;
}

/****************************************************************************
 * If the input matrices are compatible, then performs matrix subtraction	*
 * and returns a pointer to the result matrix.								*
 * Use create(), getValueAt(), and setValueAt() functions to implement this	*
 * function. If the input matrices are not compatible, return NULL.			*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *subtract(Matrix *m1, Matrix *m2) {
	
	//Not comaptible
	if(m1->rows != m2->rows || m1->columns != m2->columns)
		return NULL;
		
	Matrix *result = create(m1->rows,m1->columns);
	//calculate values
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->columns; j++) {
			setValueAt(result,i,j,getValueAt(m1,i,j)-getValueAt(m2,i,j));
		}
	}

	return result;
}

/****************************************************************************
 * Creates the transpose matrix of the input matrix and returns a pointer	*
 * to the result matrix. Use create(), getValueAt(), and setValueAt() 		*
 * functions to implement this function.									*
 * DO NOT modify the input matrix.											*
 ***************************************************************************/
Matrix *transpose(Matrix *m) {
	
	Matrix *result = create(m->columns, m->rows);
	
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->columns; j++) {
			setValueAt(result,i,j,getValueAt(m,j,i));
		}
	}
	return result;
}

/****************************************************************************
 * Creates a matrix that is the product of the given scalar value and		*
 * the input matrix and returns a pointer to the result matrix.				*
 * Use create(), getValueAt(), and setValueAt() functions to implement 		*
 * this function.															*
 * DO NOT modify the input matrix.											*
 ***************************************************************************/
Matrix *scalarMultiply(Matrix *m, int scalar) {
	
	Matrix *result = create(m->rows, m->columns);
	
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->columns; j++) {
			setValueAt(result,i,j,getValueAt(m,i,j)*scalar);
		}
	}
	
	return result;
}

/****************************************************************************
 * If the input matrices are compatible, then multiplies the input matrices	*
 * and returns a pointer to the result matrix. Use create(), getValueAt(), 	*
 * and setValueAt() functions to implement this function.					*
 *																			*
 * If the input matrices are not compatible, return NULL.					*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *multiply(Matrix *m1, Matrix *m2) {
	
	if(m1->columns != m2->rows)
    	return NULL;
  
    Matrix *result = create(m1->rows, m2->columns);
    int temp = 0;
    for (int i = 0; i < m1->rows; i++) {
		for (int j = 0; j < m2->columns; j++) {
			for (int k = 0; k < m1->columns; k++) {
				//i is the current row of the first matrix.
				//j is the current column of the second matrix.
				//k is the current column of the first matrix.
				temp += getValueAt(m1,i,k) * getValueAt(m2,k,j);
			}
			setValueAt(result,i,j,temp);
			temp = 0;
		}
	}

	return result;
}


