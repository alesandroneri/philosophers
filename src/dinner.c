#include "../includes/philo.h"

static void	to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	if (philo->id % 2 == 0)
		grab_forks(philo, philo->right_fork, philo->left_fork);
	else
		grab_forks(philo, philo->left_fork, philo->right_fork);
	printf("%ld %d is eating\n", timez(philo), philo->id);
	pthread_mutex_lock(&philo->table->state_mutex);
	philo->last_meal = timez(philo);
	pthread_mutex_unlock(&philo->table->state_mutex);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_lock(&philo->table->state_mutex);
	philo->count++;
	if (philo->count == philo->table->max_meals)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->table->state_mutex);
	leave_forks(philo, philo->right_fork, philo->left_fork);
}

static void	to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	printf("%ld %d is sleeping\n", timez(philo), philo->id);
	pthread_mutex_lock(&philo->table->state_mutex);
	usleep(philo->table->time_to_sleep * 1000);
	pthread_mutex_unlock(&philo->table->state_mutex);
}

static void	to_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	printf("%zu %d is thinking\n", timez(philo), philo->id);
}


static void	*monitor(t_table *table)
{
	int		i;
	long	time_elapsed;
	int		finished_dinner;

	while (1)
	{
		i = -1;
		finished_dinner = 0;
		while (++i < table->philo_nbr)
		{
			pthread_mutex_lock(&table->state_mutex);
			time_elapsed = current_time_ms() - table->philos[i].last_meal;
			pthread_mutex_unlock(&table->state_mutex);
			if (time_elapsed > table->time_to_die)
			{
				usleep(500);
				printf("%ld %d died\n", timez(&table->philos[i]),
					table->philos[i].id);
				table->philos[i].is_alive = 0;
				table->end = 1;
				return (NULL);
			}
			if (table->max_meals > 0
				&& table->philos[i].count == table->max_meals)
				finished_dinner++;
		}
		if (finished_dinner == table->philo_nbr)
		{
			pthread_mutex_lock(&table->state_mutex);
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}

static void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->table->state_mutex);
		if (philo->table->end || !philo->is_alive)
		{
			pthread_mutex_unlock(&philo->table->state_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->table->state_mutex);
		if (philo->table->max_meals == -1
			|| philo->count < philo->table->max_meals)
		{
			to_eat(philo);
			to_sleep(philo);
			to_think(philo);
		}
	}
	return (NULL);
}

static void	handle_thread(t_table *table, pthread_t monitor_th)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
		if (pthread_create(&table->philos[i].thread, NULL, philosopher_routine,
				&table->philos[i]) != 0)
		{
			printf("Failed to create thread for philosopher %d.\n", i);
			pthread_mutex_lock(&table->state_mutex);
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return ;
		}
	i = -1;
	while (++i < table->philo_nbr)
		pthread_join(table->philos[i].thread, NULL);
	pthread_join(monitor_th, NULL);
}

void	dinner(void *arg)
{
	pthread_t	monitor_th;
	t_table		*table;

	table = (t_table *)arg;
	if (table->philo_nbr == 0)
	{
		printf("Error the number of philosophers must be at least one.\n");
		return ;
	}
	if (pthread_create(&monitor_th, NULL, (void *)monitor, (void *)table) != 0)
	{
		printf("Thread to monitor death of philosophers failed to be created.\n");
		return ;
	}
	if (table->philo_nbr == 1)
	{
		one_philo(table);
		return ;
	}
	if (table->max_meals == 0)
	{
		printf("Error the numbers of meals must be at least one.\n");
		return ;
	}
	handle_thread(table, monitor_th);
}
