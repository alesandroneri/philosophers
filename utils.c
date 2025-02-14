#include "philo.h"

void get_fork(t_philo *philo, pthread_mutex_t fork)
{
    pthread_mutex_lock(&fork);
    printf("timestamp_in_ms %d has taken a fork\n", philo->id);
}

void put_fork(t_philo *philo, pthread_mutex_t fork)
{
    pthread_mutex_unlock(&fork);
    printf("timestamp_in_ms %d has put a fork\n", philo->id);
}

long long current_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
}
void one_philo(t_table *table)
{
    printf("timestamp_in_ms %ld %d died\n", current_time_ms, table->philosophers->id);
}
void free_resources(t_table *table)
{

}