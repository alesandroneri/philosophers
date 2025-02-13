#include <stdio.h>
#include <pthread.h>
#include "philo.h"

void init(t_table *table)
{
	table->end_routine = 0;
	table->philosophers = malloc(sizeof(t_philo) * table->philosophers_number);
	table->	
}

void new_philosopher(int philosopher)
{

}

int main(int ac, char **av)
{
    t_table table;

    //pthread_create(&philosophers, NULL, &routine, NULL);
    //pthread_join(philosophers, NULL);
    if (ac == 5 || ac == 6)
    {
        parse(ac, av, &table);
		
    }
    else
    {
        printf("Error invalid number of arguments.\n");
        return (1);
    }
    return (0);
}
