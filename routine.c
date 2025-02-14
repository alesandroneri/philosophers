#include "philo.h"
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died

void to_eat(t_table *table, t_philo *philo)
{
    get_fork(table->philosophers->right_fork);
    get_fork(table->philosophers->left_fork);
    printf("%ld %d is eating\n", current_time_in_ms, philo->id);
    philo->last_meal = current_time_in_ms();
    put_fork(table->philosophers, table->philosophers->right_fork);
    put_fork(table->philosophers, table->philosophers->left_fork);
}
void to_sleep(t_philo *philo)
{
    printf("%ld %d is sleeping\n", current_time_in_ms, philo->id);
    usleep();
}
void to_think(t_philo *philo)
{
    printf("%ld %d is thinking", current_time_in_ms, philo->id);
}

void monitor(t_table *table)
{
    int i;
    int now;

    while(1)
    {
        i = 0;
        now = current_time_in_ms;
        while(i < table->philosophers_number)
        {
            if((now - table->philosophers[i].last_meal > table->time_to_die))
            {
                printf("timestamp_in_ms %ld %d died\n", now, table->philosophers[i].id);
                table->end_routine = 1;
                return NULL;
            }
            i++;
        }
    }
    usleep(1000);
}

void dinner(t_table *table)
{
    int i;

    i = 0;
    if (table->philosophers_number == 0)
        return ;
    else if (table->philosophers_number == 1)//to_do filosofo na mesa sozinho morre
        return ;
    else if (table->nbr_max_meals == 0)//filosofos nao se alimentam caso a quantidade de refeições seja 0
        return ;
    while(!table->end_routine)
    {
        while(1)
        {
            to_eat(table, table->philosophers);
            to_sleep(table->philosophers);
            to_think(table->philosophers);
        }
    }
    
}
