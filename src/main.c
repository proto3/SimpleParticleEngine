#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <SimpleParticle/physic.h>
#include <SimpleParticle/constants.h>
#include <SimpleParticle/graphic.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int alarm_marker = 0;
void alarm_handler(int sign)
{
	alarm_marker = 1;
}
//------------------------------------//
struct arg_main
{
	int end, pause;
	pthread_mutex_t *main_lock;
	pthread_cond_t *main_cond;
};
//------------------------------------//
struct arg_worker
{
	int *cond, from, to;
	pthread_mutex_t *worker_lock;
	pthread_cond_t *physic_cond;
	pthread_cond_t *graphic_cond;
	particle **a;
	particle **b;
};
//--------------------------------------------------------------------//
void* thread_worker(void* argw)
{
	struct arg_worker *arg;
	arg = (struct arg_worker*)argw;

	while(1)//condition which stop threads
	{
		//work
		step_3(*arg->a, *arg->b, arg->from, arg->to);

		pthread_mutex_lock(arg->worker_lock);
		//--------------------------------//
			*(arg->cond)+=1;
			if(*arg->cond == N_THREAD)
			{
				pthread_cond_signal(arg->graphic_cond);
			}
			else
			pthread_cond_wait(arg->physic_cond, arg->worker_lock);
		//--------------------------------//
		pthread_mutex_unlock(arg->worker_lock);

	}
	pthread_exit(0);
}
//--------------------------------------------------------------------//
void* start(void* argm)
{
	init_graphics();
	usleep(200000);
	struct arg_main *arg;
	arg = (struct arg_main*)argm;

	int i, cond = 0;
	pthread_mutex_t worker_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t physic_cond = PTHREAD_COND_INITIALIZER;
	pthread_cond_t graphic_cond = PTHREAD_COND_INITIALIZER;

	particle *tab_a = malloc(N_PARTICLE*sizeof(particle));
	particle *tab_b = malloc(N_PARTICLE*sizeof(particle));
	init_rand();
	init(tab_a,N_PARTICLE);

	particle **a=&tab_a, **b=&tab_b, *tmp;

	//launch threads for physic engine
	struct arg_worker *argw;
	pthread_t *tids = (pthread_t*)malloc(N_THREAD*sizeof(pthread_t));
	for(i=0;i<N_THREAD;i++)
	{
		argw = (struct arg_worker *)malloc(sizeof(struct arg_worker));
		argw->cond = &cond;
		argw->from = (i*(N_PARTICLE/N_THREAD));
		argw->to = ((i+1)*(N_PARTICLE/N_THREAD))-1;
		if(i==(N_THREAD-1))
			argw->to += N_PARTICLE % N_THREAD;
		argw->worker_lock = &worker_lock;
		argw->physic_cond = &physic_cond;
		argw->graphic_cond = &graphic_cond;
		argw->a = a;
		argw->b = b;
		pthread_create(&tids[i],NULL,thread_worker,(void*)argw);
	}

	//start
	ualarm(1, 1000000/FPS);
	while(!(arg->end))
	{
		while(!arg->pause && !arg->end)
		{
			pthread_mutex_lock(&worker_lock);
			//--------------------------------//
				if(cond < N_THREAD)
				{
					pthread_cond_wait(&graphic_cond, &worker_lock);
				}
				cond = 0;
				//SWITCH TABS
				tmp = *a;
	 			*a = *b;
				*b = tmp;
				pthread_cond_broadcast(&physic_cond);
			//--------------------------------//
			pthread_mutex_unlock(&worker_lock);

			if(alarm_marker)
			{
				printf("FPS alarm tick missed\n");
			}
			while(!alarm_marker)
			{
				wait(NULL);
			}
			alarm_marker = 0;

			display_frame(tab_a, N_PARTICLE);
		}

		//pause
		if(!arg->end)
		{
			pthread_mutex_lock(arg->main_lock);
			//--------------------------------//
				if(arg->pause)
				{
					pthread_cond_wait(arg->main_cond, arg->main_lock);
				}
			//--------------------------------//
			pthread_mutex_unlock(arg->main_lock);
		}
	}

	//stop
	for(i=0;i<N_THREAD;i++)
	{
		pthread_join(tids[i],NULL);
	}
	pthread_exit(0);
}
//--------------------------------------------------------------------//
int main(int argc, char* argv[])
{
	char c;
	pthread_t tid;
	int running = 0;
	pthread_mutex_t ml = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t mc = PTHREAD_COND_INITIALIZER;

	struct arg_main arg;
	arg.end = 0;
	arg.pause = 0;
	arg.main_lock = &ml;
	arg.main_cond = &mc;

	signal(SIGALRM, alarm_handler);

	printf("Type 'h' for help\n");
	printf("--------------------\n");
	while(!arg.end)
	{
		scanf("%c",&c);
		switch(c)
		{
		case 'h':
			printf("start: s\npause: p\nquit: q\n");
			break;
		case 's':
			if(arg.pause)
			{
				printf("restart");
				running = 1;
				pthread_mutex_lock(arg.main_lock);
					arg.pause = 0;
					pthread_cond_signal(arg.main_cond);
				pthread_mutex_unlock(arg.main_lock);
			}
			else if(!running)
			{
				running = 1;
				pthread_create(&tid,NULL,start,(void*)&arg);
			}
			break;
		case 'p':
			if(running)
			{
				arg.pause = 1;
			}
			break;
		case 'q':
			arg.end = 1;
			break;
		default:
			printf("No choice matching\n");
			break;
		}
		printf("--------------------\n");
		while (c != '\n' && c != EOF)
        	c = getchar();
	}

	return 0;
}
//--------------------------------------------------------------------//
