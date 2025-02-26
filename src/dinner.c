#include "../includes/philo.h"

static void	to_eat(t_philo *philo)
{
	//if (philo->id % 2 == 0)
	//{
	pthread_mutex_lock(&philo->table->state_mutex);
	grab_forks(philo, philo->right_fork, philo->left_fork);
	pthread_mutex_unlock(&philo->table->state_mutex);
	//}
	// else
	// {
	// 	pthread_mutex_lock(&philo->table->state_mutex);
	// 	grab_forks(philo, philo->left_fork, philo->right_fork);
	// 	pthread_mutex_unlock(&philo->table->state_mutex);
	// }
	pthread_mutex_lock(&philo->table->state_mutex);
	philo->last_meal = timez(philo);
	printf("%ld %d is eating\n", philo->last_meal, philo->id);
	pthread_mutex_unlock(&philo->table->state_mutex);
	my_sleep(philo->table->time_to_eat);
	philo->count++;
	if (philo->count == philo->table->max_meals)
		philo->is_full = 1;
	leave_forks(philo, philo->right_fork, philo->left_fork);
}

static void	to_sleep(t_philo *philo)
{
	printf("%ld %d is sleeping\n", timez(philo), philo->id);
	my_sleep(philo->table->time_to_sleep);
}

static void	to_think(t_philo *philo)
{
	printf("%zu %d is thinking\n", timez(philo), philo->id);
}

static void	monitor_death_meals(t_table *table, int i, long time_elapsed, int finished_dinner)
{
	long now;
	while (++i < table->philo_nbr)
	{
		pthread_mutex_lock(&table->state_mutex);
		now = timez(&table->philos[i]);
		time_elapsed = now - table->philos[i].last_meal;
		//printf("Philosopher: %d now: %ld last_meal: %ld time_elapsed: %ld time_to_die: %ld\n", table->philos[i].id, now, table->philos[i].last_meal, time_elapsed, table->time_to_die);
		if (time_elapsed > table->time_to_die)
		{
			my_sleep(500);
			printf("%ld %d died\n", timez(&table->philos[i]), table->philos[i].id);
			table->philos[i].is_alive = 0;
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return ;
		}
		if (table->max_meals > 0 && table->philos[i].count == table->max_meals)
			finished_dinner++;
		pthread_mutex_unlock(&table->state_mutex);
	}
	if (finished_dinner == table->philo_nbr)
	{
		pthread_mutex_lock(&table->state_mutex);
		table->end = 1;
		pthread_mutex_unlock(&table->state_mutex);
		return ;
	}
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
		time_elapsed = 0;
		monitor_death_meals(table, i, time_elapsed, finished_dinner);
		if (table->end)
			return (NULL);
		usleep(500);
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
