#include "../includes/philo.h"

static void	to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end_routine)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	if (philo->id % 2 == 1)
		usleep(2000);
	if (philo->id % 2 == 0)
	{
		get_fork(philo, &philo->left_fork);
		get_fork(philo, &philo->right_fork);
	}
	else
	{
		get_fork(philo, &philo->right_fork);
		get_fork(philo, &philo->left_fork);
	}
	printf("%zu %d is eating\n", current_time_ms(), philo->id);
	pthread_mutex_lock(&philo->table->state_mutex);
	philo->last_meal = current_time_ms();
	get_time(philo->table->time_to_eat);
	philo->meals_count++;
	if (philo->meals_count == philo->table->nbr_max_meals)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->table->state_mutex);
	put_fork(philo, &philo->right_fork);
	put_fork(philo, &philo->left_fork);
}

static void	to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end_routine)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	printf("%zu %d is sleeping\n", current_time_ms(), philo->id);
	pthread_mutex_lock(&philo->table->state_mutex);
	get_time(philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->table->state_mutex);
}

static void	to_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end_routine)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	printf("%zu %d is thinking\n", current_time_ms(), philo->id);
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
		while (++i < table->philosophers_number)
		{
			pthread_mutex_lock(&table->state_mutex);
			time_elapsed = current_time_ms() - table->philosophers[i].last_meal;
			if (time_elapsed >= table->time_to_die)
			{
				printf("%zu %d died\n", current_time_ms(),
					table->philosophers[i].id);
				table->philosophers[i].is_alive = 0;
				table->end_routine = 1;
				pthread_mutex_unlock(&table->state_mutex);
				return (NULL);
			}
			if (table->nbr_max_meals > 0
				&& table->philosophers[i].meals_count == table->nbr_max_meals)
				finished_dinner++;
			pthread_mutex_unlock(&table->state_mutex);
		}
		if (finished_dinner == table->philosophers_number)
		{
			pthread_mutex_lock(&table->state_mutex);
			table->end_routine = 1;
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
		if (philo->table->end_routine || !philo->is_alive)
		{
			pthread_mutex_unlock(&philo->table->state_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->table->state_mutex);
		if (philo->table->nbr_max_meals == -1
			|| philo->meals_count < philo->table->nbr_max_meals)
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
	while (++i < table->philosophers_number)
		if (pthread_create(&table->philosophers[i].thread, NULL,
				philosopher_routine, &table->philosophers[i]) != 0)
		{
			printf("Failed to create thread for philosopher %d.\n", i);
			pthread_mutex_lock(&table->state_mutex);
			table->end_routine = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return ;
		}
	i = -1;
	while (++i < table->philosophers_number)
		pthread_join(table->philosophers[i].thread, NULL);
	pthread_join(monitor_th, NULL);
}

void	dinner(void *arg)
{
	pthread_t	monitor_th;
	t_table		*table;

	table = (t_table *)arg;
	if (table->philosophers_number == 0)
	{
		printf("Error the number of philosophers must be at least one.\n");
		return ;
	}
	if (pthread_create(&monitor_th, NULL, (void *)monitor, (void *)table) != 0)
	{
		printf("Thread to monitor death of philosophers failed to be created.\n");
		return ;
	}
	if (table->philosophers_number == 1)
	{
		one_philo(table);
		return ;
	}
	if (table->nbr_max_meals == 0)
	{
		printf("Error the numbers of meals must be at least one.\n");
		return ;
	}
	handle_thread(table, monitor_th);
}
