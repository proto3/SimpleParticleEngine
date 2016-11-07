#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <SimpleParticle/graphic.h>
#include <SimpleParticle/structures.h>
#include <SimpleParticle/constants.h>
#include <SimpleParticle/physic.h>

//------------------------------------------//
static SDL_Surface ecran;
static SDL_Surface *buffer = &ecran;
static SDL_Rect clipper;
static SDL_Surface *square;
//------------------------------------------//
void init_graphics()
{
	SDL_Init(SDL_INIT_VIDEO);
	//SDL_ShowCursor(SDL_DISABLE);
	buffer = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE /*| SDL_FULLSCREEN*/);

	square = SDL_CreateRGBSurface(SDL_HWSURFACE,P_SIZE,P_SIZE,32,0,0,0,0);

	clipper.h = square->h;
	clipper.w = square->w;

	SDL_FillRect(square, NULL, SDL_MapRGB(square->format,255,100,0));

	pthread_t em;
	pthread_create(&em,NULL,mouse_event_manager,NULL);
}
//------------------------------------------//
void* mouse_event_manager(void* a)
{
	SDL_Event event;

    while(1)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		    case SDL_QUIT:
		        exit(0);
				break;
			case SDL_MOUSEMOTION:
				move_mouse(event.motion.x,event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button==SDL_BUTTON_LEFT);
					left_clic_mouse(1);
				if(event.button.button==SDL_BUTTON_RIGHT);
					right_clic_mouse(1);
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button==SDL_BUTTON_LEFT);
					left_clic_mouse(0);
				if(event.button.button==SDL_BUTTON_RIGHT);
					right_clic_mouse(0);
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					exit(0);
				break;
		}
	}

	pthread_exit(0);
}
//------------------------------------------//
/*void setPixel(int x, int y, Uint32 coul)
{
  *((Uint32*)(buffer->pixels) + x + y * buffer->w) = coul;
}*/
//------------------------------------------//
void display_frame(particle tab[], int length)
{
	SDL_FillRect(buffer, NULL, SDL_MapRGB(buffer->format,0,0,0));

	int i;
	for(i=0;i<length;i++)
	{
			//SDL_FillRect(noir, NULL, SDL_MapRGB(noir->format,(tab[i].pos.x-get_mouse_x())/200,200,(tab[i].pos.y-get_mouse_y())/200));
		clipper.x = (int)(tab[i].pos.x);
		clipper.y = (int)(HEIGHT-tab[i].pos.y-P_SIZE);
		SDL_BlitSurface(square, NULL, buffer, &clipper);

			//setPixel(tab[i].pos.x, HEIGHT-tab[i].pos.y-P_SIZE, SDL_MapRGB(noir->format,255,100,0));
	}
	SDL_Flip(buffer);
}
//------------------------------------------//
