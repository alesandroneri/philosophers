#include "../includes/philo.h"

static void	init_forks(t_philo *philo, pthread_mutex_t *forks, int pos)
{
	int nbr;

	nbr = philo->table->philo_nbr;
	if (philo->id % 2 == 0)
	{
		philo->right_fork = &forks[pos];
		philo->left_fork = &forks[(pos + 1) % nbr];
	}
	else
	{
		philo->right_fork = &forks[(pos + 1) % nbr];
		philo->left_fork = &forks[pos];
	}
}

static void	init_philosophers(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->last_meal = current_time_ms();
		philo->is_full = 0;
		philo->is_alive = 1;
		philo->count = 0;
		philo->table = table;
		init_forks(philo, table->forks, i);
	}
}

void	init_table(t_table *table)
{
	int	i;

	table->end = 0;
	table->start = current_time_ms();
	if (pthread_mutex_init(&table->state_mutex, NULL) != 0)
		return ;
	if (pthread_mutex_init(&table->forks_mutex, NULL) != 0)
		return ;
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
		return ;
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* table->philo_nbr);
	if (!table->forks)
	{
		free(table->philos);
		return ;
	}
	i = -1;
	while (++i < table->philo_nbr)
		pthread_mutex_init(&table->forks[i], NULL);
	init_philosophers(table);
}
