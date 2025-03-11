#include "../includes/philo.h"

static void	to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	pthread_mutex_lock(&philo->right_fork->fork);
	if (!philo->table->end)
		printf("%lld %d has taken a fork\n", timez(philo), philo->id);
	pthread_mutex_lock(&philo->left_fork->fork);
	if (!philo->table->end)
		printf("%lld %d has taken a fork\n", timez(philo), philo->id);
	philo->last_meal = timez(philo);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
		printf("%lld %d is eating\n", philo->last_meal, philo->id);
	my_sleep(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->state_mutex);
	philo->count++;
	if (philo->count == philo->table->max_meals)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->right_fork->fork);
	pthread_mutex_unlock(&philo->left_fork->fork);
}

static void	to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
		printf("%lld %d is sleeping\n", timez(philo), philo->id);
	my_sleep(philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->table->state_mutex);
}

static void	to_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	if (!philo->table->end)
		printf("%lld %d is thinking\n", timez(philo), philo->id);
}

static void	monitor_death_meals(t_table *table, int i, long long time_elapsed, int *finished_dinner)
{
	long long now;

	while (++i < table->philo_nbr)
	{
		now = timez(&table->philos[i]);
		time_elapsed = now - table->philos[i].last_meal;
		// printf("philo: %d now: %lld last_meal: %lld time_elapsed: %lld time_to_die: %lld\n",
			// table->philos[i].id, now, table->philos[i].last_meal, time_elapsed, table->time_to_die);
		if (time_elapsed > table->time_to_die)
		{
			table->philos[i].is_alive = 0;
			pthread_mutex_lock(&table->state_mutex);
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
			my_sleep(10);
			printf("%lld %d died\n", timez(&table->philos[i]), table->philos[i].id);
			return ;
		}
		if (table->max_meals > 0 && table->philos[i].count == table->max_meals)
			(*finished_dinner)++;
	}
	if (*finished_dinner == table->philo_nbr)
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
	long long	time_elapsed;
	int		finished_dinner;

	while (1)
	{
		i = -1;
		finished_dinner = 0;
		time_elapsed = 0;
		monitor_death_meals(table, i, time_elapsed, &finished_dinner);
		if (table->end)
			return (NULL);
	}
}

static void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// if (philo->id % 2 != 0)
	// 	my_sleep(5);
	while (!philo->table->end)
	{
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
