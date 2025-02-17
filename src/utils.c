#include "../includes/philo.h"

void get_fork(t_philo *philo, pthread_mutex_t fork)
{
    pthread_mutex_lock(&fork);
    printf("%lld %d has taken a fork\n", current_time_ms(),philo->id);
}

void put_fork(t_philo *philo, pthread_mutex_t fork)
{
    (void)philo;
    pthread_mutex_unlock(&fork);
    //printf("%lld %d has put a fork\n", current_time_ms(),philo->id);
}

long long current_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
}
void one_philo(t_table *table)
{
    printf("%lld %d died\n", current_time_ms(), table->philosophers->id);
}
void free_resources(t_table *table)
{
    int i;
    if(table->philosophers)
        free(table->philosophers);
    i = -1;
    while(++i < table->philosophers_number)
        pthread_mutex_destroy(&table->forks[i]);
    free(table->forks);
    free(table);
}