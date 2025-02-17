#include "../includes/philo.h"

int main(int ac, char **av)
{
    t_table table;

    if (ac == 5 || ac == 6)
    {
        parse(ac, av, &table);
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
