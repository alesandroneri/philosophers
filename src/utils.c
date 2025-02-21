#include "../includes/philo.h"


void	get_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	printf("%zu %d has taken a fork\n", current_time_ms(), philo->id);
}

void	put_fork(t_philo *philo, pthread_mutex_t *fork)
{
	(void)philo;
	pthread_mutex_unlock(fork);
}
void grab_forks(t_philo *philo, pthread_mutex_t *fork_one, pthread_mutex_t *fork_two)
{
	pthread_mutex_lock(&philo->table->forks_mutex); // Trava o mutex dos garfos
    get_fork(philo, fork_one);
    get_fork(philo, fork_two);
    pthread_mutex_unlock(&philo->table->forks_mutex);
}
void leave_forks(t_philo *philo, pthread_mutex_t *fork_one, pthread_mutex_t *fork_two)
{
	put_fork(philo, fork_one);
	put_fork(philo, fork_two);
}

void	get_time(size_t milisecond)
{
	size_t	start;

	start = current_time_ms();
	while (current_time_ms() - start < milisecond)
		usleep(200);
}


size_t	current_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error gettimeofday failed.\n");
		return (1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	one_philo(t_table *table)
{
	printf("%zu %d died\n", current_time_ms(), table->philosophers->id);
}

void	free_resources(t_table *table)
{
	int i;

    if (!table)
        return ;
    if (table->forks)
    {
        i = -1;
        while (++i < table->philosophers_number)
		    pthread_mutex_destroy(&table->forks[i]);
        free(table->forks);
    }
	pthread_mutex_destroy(&table->state_mutex);
	if (table->philosophers)
		free(table->philosophers);
}