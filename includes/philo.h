
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
	long					last_meal;
	int					nbr_meals_philo_must_eat;
	int					is_full;
	int					is_alive;
	t_table				*table;
}						t_philo;

struct	s_table
{
	int					philosophers_number;
	long					time_to_die;
	long					time_to_eat;
	long					time_to_sleep;
	long					time_to_think;
	int					nbr_max_meals;
	int					start_routine;
	int					end_routine;
	pthread_mutex_t		*forks;
	t_philo				*philosophers;;
};


//parser para verificar que se os inputs estao corretos.
int	parse(int ac, char **av, t_table *table);
//init para inicializar a table, os filosofos e os garfos.
void init_table(t_table *table);
//funcao para calcular o tempo
size_t current_time_ms(void);
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

#endif