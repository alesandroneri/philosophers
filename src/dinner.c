#include "../includes/philo.h"

void to_eat(t_philo *philo)
{
    if (philo->meals_count == philo->table->nbr_max_meals)
        return ;
    get_fork(philo, philo->right_fork);
    get_fork(philo, philo->left_fork);
    printf("%lld %d is eating\n", current_time_ms(), philo->id);
    philo->meals_count++;
    usleep(philo->table->time_to_eat);
    philo->last_meal = current_time_ms();
    put_fork(philo, philo->right_fork);
    put_fork(philo, philo->left_fork);
}

void to_sleep(t_philo *philo)
{
    printf("%lld %d is sleeping\n", current_time_ms(), philo->id);
    usleep(philo->table->time_to_sleep);
}

void to_think(t_philo *philo)
{
    printf("%lld %d is thinking\n", current_time_ms(), philo->id);
    usleep(philo->table->time_to_think);
}

void *monitor(t_table *table)
{
    int i;
    int now;

    while(1)
    {
        i = 0;
        now = current_time_ms();
        while(i < table->philosophers_number)
        {
            if((now - table->philosophers[i].last_meal > table->time_to_die))
            {
                printf("%d %d died\n", now, table->philosophers[i].id);
                table->philosophers[i].is_alive = 0;
                table->end_routine = 1;
                return (NULL);
            }
            i++;
        }
    }
    usleep(1000);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;

    if (philo->id % 2 == 0)
        usleep(100);
    while (!philo->table->end_routine && philo->is_alive)
    {
        if (philo->meals_count < philo->table->nbr_max_meals)
            to_eat(philo);
        to_sleep(philo);
        to_think(philo);
    }
    return (NULL);
}

void dinner(void *arg)
{
    int i;
    pthread_t monitor_th;
    t_table *table = (t_table *)arg;

    if (table->philosophers_number == 0)
        return;
    if (pthread_create(&monitor_th, NULL, (void *)monitor, (void *)table) != 0)
    {
        printf("Thread to monitor death of philosophers failed to be created.\n");
        return;
    }
    if (table->philosophers_number == 1)
    {
        one_philo(table);
        return;
    }
    if (table->nbr_max_meals == 0)
        return;
    i = -1;
    while (++i < table->philosophers_number)
        if (pthread_create(&table->philosophers[i].thread, NULL, philosopher_routine, &table->philosophers[i]) != 0)
        {
            printf("Failed to create thread for philosopher %d.\n", i);
            table->end_routine = 1;
            return ;
        }
    i = -1;
    while (++i < table->philosophers_number)
        pthread_join(table->philosophers[i].thread, NULL);
    pthread_join(monitor_th, NULL);
}

// void dinner(void *arg)
// {
//     t_table *table;
//     int i;
//     int all_philo_ate;
//     pthread_t monitor_th;

//     table = (t_table *)arg;
//     if (pthread_create(&monitor_th, NULL, (void *)monitor, (void *)table) != 0)
//     {
//         printf("Thread to monitor death of philosophers failed to be created.\n");
//         return ;
//     }
//     if (table->philosophers_number == 0)
//         return ;
//     else if (table->philosophers_number == 1)
//     {
//         one_philo(table);
//         return ;
//     }  
//     else if (table->nbr_max_meals == 0)
//         return ;
//     while(!table->end_routine)
//     {
//         i = -1;
//         all_philo_ate = 1;
//         while(++i < table->philosophers_number)
//         {
//             if (!table->philosophers[i].is_alive)
//             {
//                 table->end_routine = 1;
//                 break ;
//             }
//             if (table->philosophers[i].meals_count < table->nbr_max_meals)
//                 all_philo_ate = 0;
//         }
//         if (all_philo_ate)
//             table->end_routine = 1;
//         i = -1;
//         while(++i < table->philosophers_number)
//         {
//             if (table->philosophers[i].is_alive)
//             {
//                 to_eat(table->philosophers);
//                 to_sleep(table->philosophers);
//                 to_think(table->philosophers);
//             }
//         } 
//     }
//     pthread_join(monitor_th, NULL);
// }
