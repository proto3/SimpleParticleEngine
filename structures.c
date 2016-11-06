#include "structures.h"

vect add(vect *a, vect *b)
{
	vect x = {a->x+b->x,a->y+b->y};
	return x;
}

vect sub(vect *a, vect *b)
{
	vect x = {a->x-b->x,a->y-b->y};
	return x;
}
