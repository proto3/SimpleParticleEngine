#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "structures.h"
#include "constants.h"

typedef struct v
{
	vect pos;
	struct v *next;
} *vortex_list;

vortex_list vl = NULL;
int mouse_x = 0, mouse_y = 0, pressed = 0;

//------------------------------------------//
int get_mouse_x()
{
	return mouse_x;
}
//------------------------------------------//
int get_mouse_y()
{
	return mouse_y;
}
//------------------------------------------//
void move_mouse(int x, int y)
{
	mouse_x = x;
	mouse_y = (HEIGHT-1)-y;
}
//------------------------------------------//
//1 clicked 0 released
void left_clic_mouse(int b)
{
	pressed = b;
	if(pressed)
	{
		vortex_list new = (vortex_list)malloc(sizeof(struct v));
		new->pos.x = mouse_x;
		new->pos.y = mouse_y;
		new->next = vl;
		vl = new;
	}
}
//------------------------------------------//
//1 clicked 0 released
void right_clic_mouse(int b)
{
	pressed = b;
}
//------------------------------------------//
void init_rand()
{
	srand(time(NULL));
}
//------------------------------------------//
void init(particle tab[], int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		tab[i].pos.x = rand()%WIDTH;
		tab[i].pos.y = rand()%HEIGHT;

		tab[i].speed.x = (rand()%5);
		tab[i].speed.y = (rand()%5);

		tab[i].weight = 1.0;
	}
}
//------------------------------------------//
void step_1(particle old_tab[], particle new_tab[], int from, int to)
{
	//VORTEX MODE

	vect rel;
	float tmp;
	int i;
	for(i=from;i<=to;i++)
	{
		new_tab[i].pos = add(&old_tab[i].pos,&old_tab[i].speed);
		//new_tab[i].speed = add(&old_tab[i].speed,&gravity);

		vortex_list cursor = vl;
		while(cursor != NULL)
		{
			rel.x = cursor->pos.x-old_tab[i].pos.x;
			rel.y = cursor->pos.y-old_tab[i].pos.y;

			//tmp = sqrt((rel.x*rel.x)+(rel.y*rel.y));
			tmp = (rel.x*rel.x)+(rel.y*rel.y);
			//if(cursor->attract)
			//{
				new_tab[i].speed.x += 10*rel.x/tmp;
				new_tab[i].speed.y += 10*rel.y/tmp;
			//}
			/*else
			{
				new_tab[i].speed.x -= rel.x/tmp;
				new_tab[i].speed.y -= rel.y/tmp;
			}*/
			cursor = cursor->next;
		}

		new_tab[i].speed.x *= 0.995;
		new_tab[i].speed.y *= 0.995;

		//REBONDS
		if(new_tab[i].pos.x < 0.0)
		{
			new_tab[i].pos.x = -new_tab[i].pos.x;
			new_tab[i].speed.x = -new_tab[i].speed.x;
		}
		else if(new_tab[i].pos.x > WIDTH)
		{
			new_tab[i].pos.x = 2*WIDTH - new_tab[i].pos.x;
			new_tab[i].speed.x = -new_tab[i].speed.x;
		}

		if(new_tab[i].pos.y < 0.0)
		{
			new_tab[i].pos.y = -new_tab[i].pos.y;
			new_tab[i].speed.y = -new_tab[i].speed.y;
		}
		else if(new_tab[i].pos.y > HEIGHT)
		{
			new_tab[i].pos.y = 2*HEIGHT - new_tab[i].pos.y;
			new_tab[i].speed.y = -new_tab[i].speed.y;
		}
	}
}
//------------------------------------------//
void step_2(particle old_tab[], particle new_tab[], int from, int to)
{
	//BUMP & GRAVITY MODE

	static vect gravity = {0.0, -(G/FPS)};

	int i;
	for(i=from;i<=to;i++)
	{
		new_tab[i].pos = add(&old_tab[i].pos,&old_tab[i].speed);
		new_tab[i].speed = add(&old_tab[i].speed,&gravity);

		new_tab[i].speed.x *= 0.995;
		new_tab[i].speed.y *= 0.995;

		//REBONDS
		if(new_tab[i].pos.x < 0.0)
		{
			new_tab[i].pos.x = 0.0;
			new_tab[i].speed.x = -new_tab[i].speed.x;
		}
		else if(new_tab[i].pos.x > WIDTH-2)
		{
			new_tab[i].pos.x = WIDTH-2;
			new_tab[i].speed.x = -new_tab[i].speed.x;
		}

		if(new_tab[i].pos.y < 0.0)
		{
			new_tab[i].pos.y = 0.0;
			new_tab[i].speed.y = -new_tab[i].speed.y;
		}
		else if(new_tab[i].pos.y > HEIGHT-2)
		{
			new_tab[i].pos.y = HEIGHT-2;
			new_tab[i].speed.y = -new_tab[i].speed.y;
		}
	}
}
//------------------------------------------//
void step_3(particle old_tab[], particle new_tab[], int from, int to)
{
	//MOUSE TARGET MODE

	vect rel;
	float tmp;
	int i;
	for(i=from;i<=to;i++)
	{
		new_tab[i].pos = add(&old_tab[i].pos,&old_tab[i].speed);

		rel.x = get_mouse_x()-old_tab[i].pos.x;
		rel.y = get_mouse_y()-old_tab[i].pos.y;

		tmp = sqrt((rel.x*rel.x)+(rel.y*rel.y)) * 0.6;

		new_tab[i].speed.x += rel.x/tmp;
		new_tab[i].speed.y += rel.y/tmp;

		new_tab[i].speed.x *= 0.99;
		new_tab[i].speed.y *= 0.99;
	}
}
