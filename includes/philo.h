
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
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	int					count;
	long long					last_meal;
	int					is_full;
	int					is_alive;
	t_table				*table;
}						t_philo;

struct	s_table
{
	int					philo_nbr;
	long long					time_to_die;
	long long					time_to_eat;
	long long					time_to_sleep;
	long long					time_to_think;
	int					max_meals;
	long long					start;
	int					end;
	pthread_mutex_t		*forks;
	pthread_mutex_t forks_mutex;
	pthread_mutex_t		state_mutex;
	t_philo				*philos;;
};

long long	timez(t_philo *philo);
//parser para verificar que se os inputs estao corretos.
int	parse(int ac, char **av, t_table *table);
//init para inicializar a table, os filosofos e os garfos.
void init_table(t_table *table);
//funcao para calcular o tempo
long long current_time_ms(void);
//funcao para calcular o tempo das acoes
void get_time(size_t milisecond);
//funcao para liberar a memoria alocada
void free_resources(t_table *table);
//funcao para pegar o garfo
void get_fork(t_philo *philo, pthread_mutex_t *fork);
//funcao para deixar o garfo na mesa
void put_fork(t_philo *philo, pthread_mutex_t *fork);
//funcao para o jantar
void dinner(void *arg);
//funcao para um filosofo
void one_philo(t_table *table);

void grab_forks(t_philo *philo, pthread_mutex_t *fork_one, pthread_mutex_t *fork_two);

void leave_forks(t_philo *philo, pthread_mutex_t *fork_one, pthread_mutex_t *fork_two);

void my_sleep(long long sleeping);

#endif