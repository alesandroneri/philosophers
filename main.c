#include <stdio.h>
#include <pthread.h>
#include "philo.h"

int	is_all_numbers(char *nb)
{
	int	i;

	if (!nb)
		return (0);
	//if (ft_strlen(nb) == 1 && (nb[0] == '-' || nb[0] == '+'))
	//	return (0);
	i = 0;
    if (nb[0] == '-')
        return (0);
	if (nb[0] == '+')
		i++;
	while (nb[i] != '\0')
	{
        if (!(nb[i] >= '0' && nb[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

//void take_fork()
//void put_fork()
//void think()
//void philo_eat()
//void philo_sleep()
int check_arguments(int ac, char **av)
{
    int		i;
	int		j;
	long	nb;

	i = 0;
	while (i < ac)
	{
		if (!ft_is_all_numbers(av[i]))
			return (0);
		i++;
	}
    return (1);
}

void *routine()
{
    printf("routine\n");
}

void new_philosopher(int philosopher)
{

}

int main(int ac, char **av)
{
    int i;
    t_philo philosophers;
    pthread_t *th;
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    //pthread_create(&philosophers, NULL, &routine, NULL);
    //pthread_join(philosophers, NULL);

    if (ac == 6)
    {

        check_arguments(ac, av);
        i = -1;
        while (++i < ac)
        {
            if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
            {
                printf("Failed to create thread\n");
                return (-1);
            }
            printf("Thread %d has started\n", i);
        }
        i = -1;
        while (++i < ac)
        {
            if(pthread_join(th[i], NULL) != 0)
            {
                printf("Failed to join thread\n");
                return (-1);
            }
            printf("Thread %d has finished execution\n", i);
        }
    }
    pthread_mutex_destroy(&mutex);
    return (0);
}