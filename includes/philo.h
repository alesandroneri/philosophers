

// pthread_create()// create a new thread: on success returns 0, on error returns an error number.
// pthread_detach()// detach a thread: on success returns 0, on error returns an error number.
// pthread_join()// join with a terminated thread: on success returns 0, on error returns an error number.
// pthread_mutex_init()// destroy and initialize a mutex: on success returns 0, on error returns an error number.
// pthread_mutex_destroy()
// destroy and initialize a mutex: on success returns 0, on error returns an error number.
// pthread_mutex_lock()// lock and unlock a mutex: on success returns 0,on error returns an error number.
// pthread_mutex_unlock()// unlock and lock a mutex: on success returns 0,on error returns an error number.
// gettimeofday()// get / set time: return 0 for success, or -1 for failure.
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int					id;
	pthread_t thread;
	pthread_mutex_t		right_fork;
	pthread_mutex_t		left_fork;
	int					meals_count;
	int					last_meal;
	int					nbr_meals_philo_must_eat;
	int					is_full;
	int					is_alive;
	t_table				*table;
}						t_philo;

struct	s_table
{
	int					philosophers_number;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_think;
	int					nbr_max_meals;
	int					start_routine;
	int					end_routine;
	pthread_mutex_t		*forks;
	t_philo				*philosophers;;
};


//parser para verificar que se os inputs estao corretos.
void	parse(int ac, char **av, t_table *table);
//init para inicializar a table, os filosofos e os garfos.
void init_table(t_table *table);
//funcao para calcular o tempo
long long current_time_ms(void);
//funcao para liberar a memoria alocada
void free_resources(t_table *table);
//funcao para pegar o garfo
void get_fork(t_philo *philo, pthread_mutex_t fork);
//funcao para deixar o garfo na mesa
void put_fork(t_philo *philo, pthread_mutex_t fork);
//funcao para o jantar
//void dinner(t_table *table);
void dinner(void *arg);
//funcao para um filosofo
void one_philo(t_table *table);

#endif