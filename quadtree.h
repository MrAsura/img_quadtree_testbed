#ifndef QUADTREE_H 
#define QUADTREE_H

/*
Define boolean values
*/
typedef enum{FALSE,TRUE} BOOL;

/*
	Define possible quality levels
*/
typedef enum{NO,LOW,MED,HIGH,ORIG} Quality_l;

/*
	Define bounds for compare functions
*/
#define LOW_BOUND 0.1
#define MED_BOUND 0.01
#define HIGH_BOUND 0.001

/*
	Define a struct to descripe a quadtree
		level: level of qt. 0 is the lowest level
		val: a value at the current level. Average of sub-trees. -1 if no value
		quality: quality level of val if val is set.
		q*: one of four sub-quads
		
		order : q1 q2
				q3 q4
*/
typedef struct quad{
	int level; 
	double val;
	Quality_l quality;
	struct quad * q1;
	struct quad * q2;
	struct quad * q3;
	struct quad * q4;
} Quadtree;

/*
	Make a quadtree using initial values in init (lowest level) of dimension dimXdim
		dim: dimensions of init. should be a power of 2
		init: a two dimensional array of doubles to be used in lowest level
		init_x: the beginning x index in the init array
		init_y: the beginning y index in the init array
*/
Quadtree * makeQuadtree( int dim, double ** init, int init_x, int init_y );

/* free memory of quadtree */
void freeQuadtree( Quadtree * qt );

/*
	Merge quads of a quadtree based on a compare function taking four values and a qualitylevel
*/
void merge( Quadtree * qt, BOOL (*compare) (double, double, double, double, Quality_l) );

/*
	Return a dimXdim array containing values based on the given quality level. dim is 2^qt->level
*/
double ** getArray( Quadtree * qt, Quality_l quality );

/*
	Function for comparing differences
*/
BOOL avgDistance(double v1, double v2, double v3, double v4, Quality_l q);
BOOL maxDistance(double v1, double v2, double v3, double v4, Quality_l q);

/* Function for calculating the ratio of compression for the quadtree */
double quadtreeRatio( Quality_l quality, Quadtree * qt );

#endif
