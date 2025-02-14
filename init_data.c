#include "philo.h"

static void init_forks(t_philo *philo, pthread_mutex_t *forks, int pos)
{
    philo->right_fork = forks[pos];
    philo->left_fork = forks[(pos + 1) % philo->table->philosophers_number];
}
// ./philo nbr_philosophers time_to_die time_to_eat time_to_sleep numbers_of_meals_a_philosopher can have
static void init_philosophers(t_table *table)
{
    t_philo *philo;
    int i;

    i = 0;
    while(i < table->philosophers_number)
    {
        philo = table->philosophers + i;
        philo->id = i + 1;
        philo->is_full = 0;
        philo->is_alive = 1;
        philo->meals_count = 0;
        philo->table = table;
        init_forks(philo, table->forks, i);
        i++;
    }
}

void init_table(t_table *table)
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
        pthread_mutex_init(&table->forks[i], NULL);
        i++;
    }
    init_philosophers(table);
}
