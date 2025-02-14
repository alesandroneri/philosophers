#include <stdio.h>
#include <pthread.h>
#include "philo.h"

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
        init_table(&table);
        dinner(&table);
		
    }
    else
    {
        printf("Error invalid number of arguments.\n");
        return (1);
    }
    return (0);
}
