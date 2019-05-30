#define _POSIX_C_SOURCE 200809L
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
FILE *file;
void print_array(int size, double arr[][size])
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            printf("%f ",arr[i][j]);
        }
        printf("\n");
    }
}
void saveToFile(long int time_naive,long int time_better,long int time_blas, int sizeOfVector)
{
    if(time_better<0 || time_naive<0 || time_blas <0 )
        return;
    fprintf(file,"%d",sizeOfVector);
    fprintf(file,";");
    fprintf(file,"%lu",time_naive);
    fprintf(file,";");
    fprintf(file,"%lu",time_better);
    fprintf(file,";");
    fprintf(file,"%lu\n",time_blas);

}
void rand_array(int size, double arr[][size])
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            arr[i][j]=(double)rand()/RAND_MAX;
        }
    }
}
void init_array(int size, double arr[][size])
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            arr[i][j]=0.0;
        }
    }
}
void multiply_naive(int size, double matrix1[][size], double matrix2[][size], double result[][size])
{
    for(int i =0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            for(int k=0;k<size;k++)
            {
                result[i][j]+=(matrix1[i][k]*matrix2[k][j]);
            }
        }
    }
}
void multiply_better(int size, double matrix1[][size], double matrix2[][size], double result[][size])
{
    for(int j=0;j<size;j++)
    {
        for(int k=0;k<size;k++)
        {
            for(int i=0;i<size;i++)
            {
                result[i][j]+=(matrix1[i][k]*matrix2[k][j]);
            }
        }
    }
}


int main()
{
    int size;
    file=fopen("times.csv","w");
    struct timespec before,after;

    for(int i=1;i<20;i++) {
        for(int k=0;k<10;k++)
        {
        size = i * 20;
        double array1[size][size];
        double array2[size][size];
        double result[size][size];
        rand_array(size, array1);
        rand_array(size, array2);
        init_array(size, result);

        clock_gettime(CLOCK_REALTIME, &before);
        multiply_naive(size, array1, array2, result);
        clock_gettime(CLOCK_REALTIME, &after);

        long int time_naive = (long) ((int)(after.tv_sec - before.tv_sec) * pow(10, 9) + (after.tv_nsec - before.tv_nsec));

        init_array(size, result);

        clock_gettime(CLOCK_REALTIME, &before);
        multiply_better(size, array1, array2, result);
        clock_gettime(CLOCK_REALTIME, &after);
        long int time_better = (long) ((int)(after.tv_sec - before.tv_sec) * pow(10, 9) + (after.tv_nsec - before.tv_nsec));

        init_array(size, result);
        gsl_matrix_view AA = gsl_matrix_view_array((double *) array1, size, size);
        gsl_matrix_view BB = gsl_matrix_view_array((double *) array2, size, size);
        gsl_matrix_view CC = gsl_matrix_view_array((double *) result, size, size);


        clock_gettime(CLOCK_REALTIME, &before);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &AA.matrix, &BB.matrix, 0.0, &CC.matrix);
        clock_gettime(CLOCK_REALTIME, &after);
        long int time_gsl = (long) ((int)(after.tv_sec - before.tv_sec) * pow(10, 9) + (after.tv_nsec - before.tv_nsec));

        saveToFile(time_naive, time_better, time_gsl, size);
    }
    }
    fclose(file);
    return 0;
}