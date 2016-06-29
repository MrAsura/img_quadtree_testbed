#include "mex.h"
#include "quadtree.h"
#include "quadtree.c"

/* Transform in to a hXw array pass a null out. */
void reshapeM(double * in, double ** out, mwSize h, mwSize w);

/* Return a hXw matrix in as a 1xh*w matrix out that is pre allocated */
void flattenM(double ** in, double * out, mwSize h, mwSize w);

void freeM( double ** in );

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double * inMatrix;
	double * outMatrix1;
	double * outMatrix2;
	double * outMatrix3;
	double * outMatrix4;
	double * outMatrix5;
	double * outMatrix6;
	double * rMatrix;
	mwSize dim;
	
	/* test inputs */
	if( nrhs != 1 )
	{
		mexErrMsgIdAndTxt( "MyToolbox:arrayProduct:nrhs",
						   "One input required." );
	}

	if( !mxIsDouble( prhs[0] ) ||
		mxIsComplex( prhs[0] ) )
	{
		mexErrMsgIdAndTxt( "MyToolbox:arrayProduct:notDouble",
						   "Input matrix must be type double." );
	}

	/* Read input */
	inMatrix = mxGetPr(prhs[0]);
	dim = mxGetN(prhs[0]);
	
	
	double ** matrix;
	matrix = malloc( dim * sizeof( double * ) );
	int i, j;
	for( i = 0; i < dim; ++i )
	{
		matrix[i] = malloc( dim * sizeof( double ) );
		for( j = 0; j < dim; ++j )
		{
			matrix[i][j] = inMatrix[dim*i + j];
		}
	}

	
	/* Create output for all matrices */
	plhs[0] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[2] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[3] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[4] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[5] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[1] = mxCreateDoubleMatrix(dim,dim, mxREAL );
	plhs[6] = mxCreateDoubleMatrix( 1, 6, mxREAL );
	
	outMatrix1 = mxGetPr(plhs[0]);
	outMatrix2 = mxGetPr(plhs[1]);
	outMatrix3 = mxGetPr(plhs[2]);
	outMatrix4 = mxGetPr(plhs[3]);
	outMatrix5 = mxGetPr(plhs[4]);
	outMatrix6 = mxGetPr(plhs[5]);
	rMatrix = mxGetPr( plhs[6] );
	
	/* Calculate data */
	double ** inMatrix1;
	double ** inMatrix2;
	double ** inMatrix3;
	double ** inMatrix4;
	double ** inMatrix5;
	double ** inMatrix6;
	

	
	Quadtree * qt1 = makeQuadtree((int)dim,matrix,(int)0,(int)0);
	Quadtree * qt2 = makeQuadtree((int)dim,matrix,(int)0,(int)0);
	
	merge(qt1,avgDistance);
	merge(qt2,maxDistance);
	
	inMatrix1 = getArray(qt1,LOW);
	inMatrix2 = getArray(qt1,MED);
	inMatrix3 = getArray(qt1,HIGH);
	inMatrix4 = getArray(qt2,LOW);
	inMatrix5 = getArray(qt2,MED);
	inMatrix6 = getArray(qt2,HIGH);

	rMatrix[0] = quadtreeRatio( LOW, qt1 );
	rMatrix[1] = quadtreeRatio( MED, qt1 );
	rMatrix[2] = quadtreeRatio( HIGH, qt1 );
	rMatrix[3] = quadtreeRatio( LOW, qt2 );
	rMatrix[4] = quadtreeRatio( MED, qt2 );
	rMatrix[5] = quadtreeRatio( HIGH, qt2 );

	freeQuadtree(qt1);
	freeQuadtree(qt2);
	
	freeM( matrix );

	flattenM(inMatrix1,outMatrix1,dim,dim);
	flattenM(inMatrix2,outMatrix2,dim,dim);
	flattenM(inMatrix3,outMatrix3,dim,dim);
	flattenM(inMatrix4,outMatrix4,dim,dim);
	flattenM(inMatrix5,outMatrix5,dim,dim);
	flattenM(inMatrix6,outMatrix6,dim,dim); 

	freeM( inMatrix1 );
	freeM( inMatrix2 );
	freeM( inMatrix3 );
	freeM( inMatrix4 );
	freeM( inMatrix5 );
	freeM( inMatrix6 );
}

void freeM( double ** in )
{
	int i;
	for( i = 0; i < sizeof(in)/sizeof(double *); i++ )
	{
		free( in[i] );
	}

	free( in );
}

void flattenM(double ** in, double * out, mwSize h, mwSize w)
{
	int i,j;
	for(i=0; i < w; ++i)
	{
		for(j=0; j < h; ++j)
		{
			out[h*i+j] = in[i][j];
		}
	}
}