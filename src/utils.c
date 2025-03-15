#include "../includes/philo.h"

void	get_fork(t_philo *philo, t_fork *fork)
{

	pthread_mutex_lock(&fork->fork);
	pthread_mutex_lock(&philo->table->print_mutex);
	if(!philo->table->end)
		printf("%lld %d has taken a fork\n", timez(philo), philo->id);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

void	put_fork(t_philo *philo, t_fork *fork)
{
	(void)philo;
	pthread_mutex_unlock(&fork->fork);
}

void grab_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two)
{
	if (philo->is_alive && !philo->is_full)
	{
		if (!philo->table->end)
		{
    		get_fork(philo, fork_one);
    		get_fork(philo, fork_two);
		}
	}
}

void leave_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two)
{
	put_fork(philo, fork_one);
	put_fork(philo, fork_two);
}

void	get_time(long long milisecond)
{
	long long	start;

	start = current_time_ms();
	while (current_time_ms() - start < milisecond)
		usleep(200);
}

void my_sleep(long long sleeping)
{
	long long start;
	long long current;
	long long remaining;

	start = current_time_ms();
	current = start;
	while ((remaining = (start + sleeping) - current) > 0)
	{
		if (remaining > 10)
			usleep(remaining * 100);
		else
			usleep(150);
		current = current_time_ms();
	}
}

// void my_sleep(long long sleeping)
// {
// 	long long start;
	
// 	start = current_time_ms();
// 	while (current_time_ms() < start + sleeping)
// 		;
// }

long long	timez(t_philo *philo)
{
	return (current_time_ms() - philo->table->start);
}

long long	current_time_ms(void)
{
	struct timeval	time;
	long long mili;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error gettimeofday failed.\n");
		return (1);
	}
	mili = time.tv_sec * 1000;
	mili += time.tv_usec / 1000;
	return (mili);
}

void	*one_philo(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	get_fork(philo, philo->right_fork);
	my_sleep(philo->table->time_to_die);
	philo->is_alive = 0;
	philo->table->end = 1;
	printf("%lld %d died\n", timez(philo), philo->id);
	return (NULL);
}
void handle_one_philo(t_table *table)
{
	if (pthread_create(&table->philos[0].thread, NULL, one_philo, &table->philos[0]) != 0)
	{
		printf("Failed to create thread for philosopher 1.\n");
		return ;
	}
	pthread_join(table->philos[0].thread, NULL);
}


// void	one_philo(t_table *table)
// {
// 	get_fork(table->philos, table->philos->right_fork);
// 	my_sleep(table->time_to_die);
// 	printf("%lld %d died\n", timez(table->philos), table->philos->id);
// }

void	free_resources(t_table *table)
{
	int i;

    if (!table)
        return ;
    if (table->forks)
    {
        i = -1;
		while (++i < table->philo_nbr)
			pthread_mutex_unlock(&table->forks[i].fork);
		i = -1;
        while (++i < table->philo_nbr)
		{
			pthread_mutex_unlock(&table->forks[i].fork);
		    pthread_mutex_destroy(&table->forks[i].fork);
			//pthread_mutex_destroy(&table->philos[i].meal_mutex);
		}
        free(table->forks);
    }
	pthread_mutex_unlock(&table->state_mutex);
	pthread_mutex_destroy(&table->state_mutex);
	//pthread_mutex_unlock(&table->forks_mutex);
	//pthread_mutex_destroy(&table->forks_mutex);
	if (table->philos)
		free(table->philos);
	table = NULL;
}
