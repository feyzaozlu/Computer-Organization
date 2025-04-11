/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following team_t struct
 */
team_t team = {

        "e258086",      /* First student ID */
        "FEYZA ÖZLÜ",       /* First student name */

};


/********************
 * NORMALIZATION KERNEL
 ********************/

/****************************************************************
 * Your different versions of the normalization functions go here
 ***************************************************************/

/*
 * naive_normalize - The naive baseline version of convolution
 */
char naive_normalize_descr[] = "naive_normalize: Naive baseline implementation";
void naive_normalize(int dim, float *src, float *dst) {
    int i, j;
    float min, max;
    min = src[0];
    max = src[0];

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {

            if (src[RIDX(i, j, dim)] < min) {
                min = src[RIDX(i, j, dim)];
            }
            if (src[RIDX(i, j, dim)] > max) {
                max = src[RIDX(i, j, dim)];
            }
        }
    }

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            dst[RIDX(i, j, dim)] = (src[RIDX(i, j, dim)] - min) / (max - min);
        }
    }
}

/*
 * normalize - Your current working version of normalization
 * IMPORTANT: This is the version you will be graded on
 */
char normalize_descr[] = "Normalize: Current working version";
void normalize(int dim, float *src, float *dst)
{
    int i, j, i_dim, constant2;
    float val1, val2, constant;
    float min = src[0]; 
    float max = min;

    for(i=0; i<dim; i++){
        i_dim = i*dim;
        for(j=0; j<dim; j+=2){
            val1 = src[i_dim+j];
            val2 = src[i_dim+j+1];
            if (val1<min) min=val1;
            if (val1>max) max=val1;
            if (val2<min) min=val2;
            if (val2>max) max=val2;
        }
    }

    constant = 1.0f/(max-min);

    for(i=0; i<dim; i++){
        i_dim = i*dim;
        for(j=0; j<dim; j+=4){
            constant2 = i_dim+j;
            dst[constant2]=(src[constant2]-min)*constant;
            dst[constant2+1]=(src[constant2+1]-min)*constant;
            dst[constant2+2]=(src[constant2+2]-min)*constant;
            dst[constant2+3]=(src[constant2+3]-min)*constant;
        }
    }
}

/*********************************************************************
 * register_normalize_functions - Register all of your different versions
 *     of the normalization functions  with the driver by calling the
 *     add_normalize_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_normalize_functions() {
    add_normalize_function(&naive_normalize, naive_normalize_descr);
    add_normalize_function(&normalize, normalize_descr);
    /* ... Register additional test functions here */
}




/************************
 * KRONECKER PRODUCT KERNEL
 ************************/

/********************************************************************
 * Your different versions of the kronecker product functions go here
 *******************************************************************/

/*
 * naive_kronecker_product - The naive baseline version of k-hop neighbours
 */
char naive_kronecker_product_descr[] = "Naive Kronecker Product: Naive baseline implementation";
void naive_kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod) {
    int i, j, k, l;
    for (i = 0; i < dim1; i++) {
        for (j = 0; j < dim1; j++) {
            for (k = 0; k < dim2; k++) {
                for (l = 0; l < dim2; l++) {
                    prod[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)] = mat1[RIDX(i, j, dim1)] * mat2[RIDX(k, l, dim2)];
                }
            }
        }
    }
}



/*
 * kronecker_product - Your current working version of kronecker_product
 * IMPORTANT: This is the version you will be graded on
 */
char kronecker_product_descr[] = "Kronecker Product: Current working version";
void kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod)
{
    int i, j, k, l, constant2, constant3, mult_with_constant;
    float mat1_value;
    int constant = dim1*dim2;
    float *prod_ptr;
    float *mat2_ptr;

    for(i=0; i<dim1; i++){
        constant2 = i*dim1;
        constant3 = i*dim2;
        for(k=0; k<dim2; k++){
            mult_with_constant = (constant3+k)*constant;
            for (j=0; j<dim1; j++){
                mat1_value = mat1[constant2+j];
                prod_ptr = prod+mult_with_constant+(j*dim2); 
                mat2_ptr = mat2+(k*dim2);
                for(l=0; l<dim2; l+=4){
                    prod_ptr[0] = mat1_value * mat2_ptr[0];
                    prod_ptr[1] = mat1_value * mat2_ptr[1];
                    prod_ptr[2] = mat1_value * mat2_ptr[2];
                    prod_ptr[3] = mat1_value * mat2_ptr[3];
                    prod_ptr+=4;
                    mat2_ptr+=4;
                }
            }
        }
    }
}

/******************************************************************************
 * register_kronecker_product_functions - Register all of your different versions
 *     of the kronecker_product with the driver by calling the
 *     add_kronecker_product_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 ******************************************************************************/

void register_kronecker_product_functions() {
    add_kronecker_product_function(&naive_kronecker_product, naive_kronecker_product_descr);
    add_kronecker_product_function(&kronecker_product, kronecker_product_descr);
    /* ... Register additional test functions here */
}

