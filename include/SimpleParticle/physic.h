#include "structures.h"

#ifndef H_GL_PHYSIC
#define H_GL_PHYSIC
//------------------------------------------//
void move_mouse(int x, int y);
int get_mouse_x();
int get_mouse_y();
void left_clic_mouse(int b);
void right_clic_mouse(int b);
void init_rand();
void init(particle tab[], int length);
void step_1(particle old_tab[], particle new_tab[], int from, int to);
void step_2(particle old_tab[], particle new_tab[], int from, int to);
void step_3(particle old_tab[], particle new_tab[], int from, int to);
//------------------------------------------//
#endif
