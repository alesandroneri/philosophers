#include "philo.h"

void philo(t_table *table)
{
    
}

void table(t_table *table)
{
    int i;
    table->end_routine = 0;
    table->philosophers = (t_philo *)malloc(sizeof(t_philo) * table->philosophers_number);
    if (!table->philosophers)
        return ;
    table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table->philosophers_number);
    if (!table->forks)
    {
        free(table->philosophers);
        return ;
    }
    i = 0;
    while(i < table->philosophers_number)
    {
        &table->forks[i] = (pthread_mutex_t)malloc(sizeof(pthread_mutex_t) * );
        i++;
    }
    void philo(t_table *table);
}
