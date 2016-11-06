#ifndef H_GL_STRUCTURES
#define H_GL_STRUCTURES
//------------------------------------------//
typedef struct
{
	float x,y;
} vect;
//------------------------------------------//
typedef struct
{
	vect pos;
	vect speed;
	float weight;
	//size,shape,...
} particle;
//------------------------------------------//
typedef struct link
{
	int a,b; //particle indexes
	//void (strength)(int,int); //strength function
	struct a* next;
} *links;
//------------------------------------------//
vect add(vect *a, vect *b);
//------------------------------------------//
vect sub(vect *a, vect *b);
//------------------------------------------//
#endif
