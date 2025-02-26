#include "../includes/philo.h"

// void monitor(t_table *table)
// {
//     int     i;
//     long    time_elapsed;
//     int     finished_dinner;

//     while (!table->end)
//     {
//         i = -1;
//         finished_dinner = 0;
//         while (++i < table->philo_nbr)
//         {
//             pthread_mutex_lock(&table->state_mutex);
//             time_elapsed = current_time_ms() - table->philos[i].last_meal;
//             pthread_mutex_unlock(&table->state_mutex);
            
//             if (time_elapsed > table->time_to_die)
//             {
//                 usleep(500);
//                 printf("%ld %d died\n", timez(&table->philos[i]), table->philos[i].id);
//                 table->philos[i].is_alive = 0;
//                 table->end = 1;
//                 return ;
//             }
//             if (table->max_meals > 0 && table->philos[i].count == table->max_meals)
//                 finished_dinner++;
//         }
//         if (finished_dinner == table->philo_nbr)
//         {
//             pthread_mutex_lock(&table->state_mutex);
//             table->end = 1;
//             pthread_mutex_unlock(&table->state_mutex);
//             return ;
//         }
//         usleep(1000);
//     }
// }

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		if (parse(ac, av, &table) == 0)
            return (1);
		init_table(&table);
		dinner(&table);
		free_resources(&table);
	}
	else
	{
		printf("Error invalid number of arguments.\n");
		return (1);
	}
	return (0);
}
