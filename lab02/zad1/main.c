#define _POSIX_C_SOURCE 200809L
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

FILE *file;

double *randVector(int size)
{
    double *vector=malloc(size*sizeof(double));
    for(int i=0;i<size;i++)
    {
        vector[i]=(double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1

    }
    return vector;
}
void saveToFile(long int timeVector,long int timeMatrix, int sizeOfVector)
{
    fprintf(file,"%d",sizeOfVector);
    fprintf(file,";");
    fprintf(file,"%ld",timeVector);
    fprintf(file,";");
    fprintf(file,"%ld\n",timeMatrix);
}
int main()
{
    file = fopen("dane.csv","w");
    srand ( time ( NULL));
    int sizes[]={50,100,200,500,1000,2000};
    double *a,*b;
    int size;
    long ms;
    struct timespec before,after;
    for(int i=0;i<6;i++)
    {
        size=sizes[i];
        for(int j=0;j<10;j++)
        {
            a=randVector(size);
            b=randVector(size);

            
            double result[size];
            gsl_vector *A = gsl_vector_alloc(size);
            gsl_vector *B = gsl_vector_alloc(size);
            gsl_matrix* M = gsl_matrix_alloc(size,size);

            gsl_matrix_set_all(M,3);

            A->data=a;
            B->data=b;

            clock_gettime(CLOCK_REALTIME, &before);
            gsl_blas_ddot(A,B,result); 
            clock_gettime(CLOCK_REALTIME, &after);
            
            long int timeVector = after.tv_nsec-before.tv_nsec;
            

            clock_gettime(CLOCK_REALTIME, &before);
            gsl_blas_dgemv(CblasNoTrans,1.0,M,A,0,B);
            clock_gettime(CLOCK_REALTIME, &after);

            long int timeMatrix = after.tv_nsec-before.tv_nsec;
            saveToFile(timeVector,timeMatrix,size);
      
        }
    }
    fclose(file);
    return 0;
}