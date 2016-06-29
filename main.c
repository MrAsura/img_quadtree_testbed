#include "quadtree.h"

int main()
{
	double m[3][3] = {{1.0,2.0,3.0 },{ 1.0,2.0,3.0 },{ 1.0,2.0,3.0 } };

	Quadtree * qt = makeQuadtree( 3, m, 0, 0 );

	system( "pause" );

	return 0;
}

