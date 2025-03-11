#include "../includes/philo.h"

void	get_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&fork->fork);
	if(!philo->table->end)
		printf("%lld %d has taken a fork\n", timez(philo), philo->id);
}

void	put_fork(t_philo *philo, t_fork *fork)
{
	(void)philo;
	pthread_mutex_unlock(&fork->fork);
}

void grab_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two)
{
	if (philo->is_alive)
	{
    	get_fork(philo, fork_one);
    	get_fork(philo, fork_two);
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
	
	start = current_time_ms();
	while (current_time_ms() < start + sleeping)
		;
}

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

void	one_philo(t_table *table)
{
	get_fork(table->philos, table->philos->right_fork);
	printf("%lld %d died\n", timez(table->philos), table->philos->id);
}

void	free_resources(t_table *table)
{
	int i;

    if (!table)
        return ;
    if (table->forks)
    {
        i = -1;
        while (++i < table->philo_nbr)
		    pthread_mutex_destroy(&table->forks[i].fork);
        free(table->forks);
    }
	pthread_mutex_destroy(&table->state_mutex);
	pthread_mutex_destroy(&table->forks_mutex);
	if (table->philos)
		free(table->philos);
}