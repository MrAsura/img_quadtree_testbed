#include "quadtree.h"

#include <math.h>
#include <stdlib.h>

Quadtree * makeQuadtree( int dim, double ** init, int init_x, int init_y )
{
	/* Top level is determined by log2(dim) */
	int top_level = (int) log2 ( (double) dim );
	
	/* init level of qt */
	Quadtree * quad = (Quadtree *) malloc( sizeof( Quadtree ) );
	quad->level = top_level;
	
	/* check if bottom reached */
	if( top_level == 0 ) 
	{
		quad->val = init[init_x][init_y];
		quad->quality = ORIG;
		
		return quad;
	}
	else
	{
		quad->val = -1.0;
		quad->quality = NO;
	}
	
	/* recursively create sub-quads
		divide dimension by two and make call with new index of init depending on the sub-quad being constructed
	*/
	int sub_dim = (int) dim/2;
	quad->q1 = makeQuadtree( sub_dim, init, init_x, init_y );
	quad->q2 = makeQuadtree( sub_dim, init, init_x+sub_dim, init_y );
	quad->q3 = makeQuadtree( sub_dim, init, init_x, init_y+sub_dim );
	quad->q4 = makeQuadtree( sub_dim, init, init_x+sub_dim, init_y+sub_dim );
	
	return quad;
}

void freeQuadtree( Quadtree * qt )
{
	if( qt->level == 0 ) return;
	
	/* recursively free sub-quads and then parent quad */
	freeQuadtree( qt->q1 );
	freeQuadtree( qt->q2 );
	freeQuadtree( qt->q3 );
	freeQuadtree( qt->q4 );
	
	free( qt );
}

Quality_l minQ( Quality_l q1, Quality_l q2 )
{
	return q1 < q2 ? q1 : q2;
}

Quality_l minQQ( Quality_l q1, Quality_l q2, Quality_l q3, Quality_l q4 )
{
	return minQ( minQ( q1, q2 ), minQ( q3, q4 ) );
}

void merge( Quadtree * qt, BOOL ( *compare ) ( double, double, double, double, Quality_l ) )
{
	/* start from level 0 and merge sub-quads if the compare function returns true */
	if( qt->level > 1 )
	{
		merge(qt->q1,compare);
		merge(qt->q2,compare);
		merge(qt->q3,compare);
		merge(qt->q4,compare);
	}
	
	double q1val = qt->q1->val;
	double q2val = qt->q2->val;
	double q3val = qt->q3->val;
	double q4val = qt->q4->val;
	
	Quality_l min_q = minQQ( qt->q1->quality, qt->q2->quality, qt->q3->quality, qt->q4->quality );

	/* Go through all levels. If a higher level returns true the lower ones should too. Set quality to the lowest of sub-quads */
	if( min_q == NO ) return;
	else if( compare(q1val,q2val,q3val,q4val,HIGH) )
	{
		qt->quality = minQ(min_q,HIGH);
		qt->val = (q1val+q2val+q3val+q4val)/4;
	}
	else if( compare(q1val,q2val,q3val,q4val,MED) )
	{
		qt->quality = minQ( min_q, MED);
		qt->val = (q1val+q2val+q3val+q4val)/4;
	}
	else if( compare(q1val,q2val,q3val,q4val,LOW) )
	{
		qt->quality = minQ( min_q, LOW );
		qt->val = (q1val+q2val+q3val+q4val)/4;
	}
}

/* Update the given array by recursively going down the quadtree untill quality level is found*/
void updateArray( double ** arr, Quality_l quality, Quadtree * qt, int init_x, int init_y )
{
	int dim = (int) ( pow( 2, qt->level ) );

	/* if qt quality matches given or is above save it */	
	if( NO != qt->quality && qt->quality >= quality )
	{
		/*populate the relevant part of the array with current value for a squire given by level*/
		int i,j;
		for( i = 0; i < dim; ++i )
		{
			for( j = 0; j < dim; ++j )
			{
				arr[init_x+i][init_y+j] = qt->val;
			}
		}
	}
	else
	{
		/* go deeper to find the correct level*/
		updateArray( arr, quality, qt->q1, init_x, init_y );
		updateArray( arr, quality, qt->q2, init_x +(int)dim/2, init_y );
		updateArray( arr, quality, qt->q3, init_x, init_y + (int) dim / 2 );
		updateArray( arr, quality, qt->q4, init_x + (int) dim / 2, init_y + (int) dim / 2 );
	}
}

double ** getArray( Quadtree * qt, Quality_l quality )
{
	int dim = (int) pow(2,qt->level);
	
	/* init a new dimXdim array */
	double ** arr = (double **) malloc( dim*sizeof(double *));

	int i;
	for( i = 0; i < dim; ++i )
	{
		arr[i] = (double *) malloc( dim * sizeof( double ) );
	}

	updateArray( arr, quality, qt, 0, 0 );

	return arr;
}

BOOL avgDistance(double v1, double v2, double v3, double v4, Quality_l q)
{
	/* Calculate the average of the values and calculate the distance to it */
	double avg = (v1+v2+v3+v4)/4;
	
	double bound;
	/* Choose bound based on quality */
	switch(q){
		case LOW:
			bound = LOW_BOUND;
			break;
		case MED:
			bound = MED_BOUND;
			break;
		case HIGH:
			bound = HIGH_BOUND;
			break;
		default:
			return FALSE;
	}
	
	return (sqrt( pow( avg - v1, 2 ) + pow( avg - v2, 2 ) + pow( avg - v3, 2 ) + pow( avg - v4, 2 ) ) < bound) ? TRUE : FALSE;
}

BOOL maxDistance(double v1, double v2, double v3, double v4, Quality_l q)
{
	/* Calculate the max distance */
	double max1 = v1 > v2 ? v1 : v2;
	double max2 = v3 > v4 ? v3 : v4;
	
	double bound;
	/* Choose bound based on quality */
	switch(q){
		case LOW:
			bound = LOW_BOUND;
			break;
		case MED:
			bound = MED_BOUND;
			break;
		case HIGH:
			bound = HIGH_BOUND;
			break;
		default:
			return FALSE;
	}
	
	return (fabs( max1 - max2 ) < bound) ? TRUE : FALSE;
}

int countBottomQuads( Quality_l quality, Quadtree * qt )
{
	/* if qt quality matches given or is above save it */
	if( NO != qt->quality && qt->quality >= quality )
	{
		/* Bottom quad reached return 1 */
		return 1;
	}
	else
	{
		/* go deeper to find the correct level*/
		int i = 0;
		i += countBottomQuads( quality, qt->q1 );
		i += countBottomQuads( quality, qt->q2 );
		i += countBottomQuads( quality, qt->q3 );
		i += countBottomQuads( quality, qt->q4 );
		return i;
	}
}

double quadtreeRatio( Quality_l quality, Quadtree * qt )
{
	/* Calculate original number of pixels */
	double orig = pow( 4, qt->level );

	/* Calculate quadtree bottom nodes for quality */
	double qt_nodes = (double) countBottomQuads( quality, qt );

	return qt_nodes/orig;
}