
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_table	t_table;

typedef struct s_fork
{
	pthread_mutex_t fork;
	int fork_id;
} t_fork ;

typedef struct s_philo
{
	int					id;
	pthread_t thread;
	pthread_mutex_t meal_mutex;//controlar o acesso das refeições
	t_fork		*right_fork;
	t_fork		*left_fork;
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
	//long long					time_to_think;
	int					max_meals;
	long long					start;
	int					end;
	t_fork		*forks;
	//pthread_mutex_t forks_mutex;// tirar
	pthread_mutex_t		state_mutex;
	pthread_mutex_t print_mutex;
	t_philo				*philos;;
};
//funcao para pegar o tempo
long long	timez(t_philo *philo);
//parser para verificar que se os inputs estao corretos.
int	parse(int ac, char **av, t_table *table);
//init para inicializar a table, os filosofos e os garfos.
void init_table(t_table *table);
//funcao para calcular o tempo
long long current_time_ms(void);
//funcao para calcular o tempo das acoes
void get_time(long long milisecond);
//funcao para liberar a memoria alocada
void free_resources(t_table *table);
//funcao para pegar o garfo
void get_fork(t_philo *philo, t_fork *fork);
//funcao para deixar o garfo na mesa
void put_fork(t_philo *philo, t_fork *fork);
//funcao para o jantar
void dinner(void *arg);
//funcao para um filosofo
//void one_philo(t_table *table);
//funcao para pegar os garfos
void grab_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two);
//funcao para colocar os garfos de volta na mesa
void leave_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two);
//funcao para reproduzir o funcionamento do usleep
void my_sleep(long long sleeping);
//funcao para lidar com um unico filosofo
void handle_one_philo(t_table *table);

#endif