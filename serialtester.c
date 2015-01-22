/*
The serial version program for matrix multiplication.
It will load the data generated by the matrixgen.c 
program and calculate the result. Finally it will compare 
the result with the result stored in the file "data_out".
We will test the correctness of your program by this 
program.
*/


#include <stdio.h>
#include <stdlib.h>

#define M 2048

int A[M][M], B[M][M], C[M][M];

int main (int argc, char* argv[])
{
	int n;
	int i,j,k;
	FILE* fp;
	int temp,flag=1;
	
	if ((fp=fopen("data_input","r"))==NULL)
	{
		printf("Error openning the input.\n");
		return 1;
	}
	fscanf(fp,"%d\n\n",&n);
	/*A=malloc(n*n*sizeof(int));
	B=malloc(n*n*sizeof(int));
	C=malloc(n*n*sizeof(int));*/
	for (i=0;i<n;i++)
		for(j=0;j<n;j++)
			fscanf(fp,"%d", &A[i][j]);
	for (i=0;i<n;i++)
		for(j=0;j<n;j++)
			fscanf(fp,"%d", &B[i][j]);
	fclose(fp);
/*Calculating*/
	for (i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			C[i][j]=0;
			for (k=0;k<n;k++)
				C[i][j]+=A[i][k]*B[k][j];
		}	
/*Testing*/
	if ((fp=fopen("data_output","r"))==NULL)
	{
		printf("Fail to load the output data.\n");
		return 1;
	}
	fscanf(fp,"%d\n\n",&temp);
	if (temp!=n)
	{
		printf("Error, the output dimension did not match the input.\n");
		return 1;
	}
	for (i=0;i<n && flag==1;i++)
		for(j=0;j<n && flag==1;j++)
		{
			fscanf(fp,"%d",&temp);
			if (temp!=C[i][j])
				flag=0;
		}
	if (flag==1)
		printf("The result is correct!\n");
	else
		printf("The result is wrong.\n");

	return 0;
}