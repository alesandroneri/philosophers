/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:23 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:15:40 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static long	ft_atol(const char *nptr)
{
	long	result;
	long	sign;
	long	i;

	result = 0;
	sign = 1;
	i = 0;
	while (nptr[i] <= 32)
		if (nptr[i] == '\e' || nptr[i++] == '\0')
			return (0);
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		if (nptr[i + 1] == '-' || nptr[i + 1] == '+')
			return (0);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - 48);
		i++;
	}
	return (result * sign);
}

static int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (nptr[i] <= 32)
		if (nptr[i] == '\e' || nptr[i++] == '\0')
			return (0);
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		if (nptr[i + 1] == '-' || nptr[i + 1] == '+')
			return (0);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	is_all_numbers(char *nb)
{
	int	i;

	if (!nb)
		return (0);
	if ((nb[0] == '-' || nb[0] == '+') && nb[1] == '\0')
		return (printf("Error only numbers must be passed as arguments.\n"), 0);
	i = 0;
	if (nb[0] == '-')
	{
		printf("Error negative numbers are invalid inputs.\n");
		return (0);
	}
	if (nb[0] == '+')
		i++;
	while (nb[i] != '\0')
	{
		if (!(nb[i] >= '0' && nb[i] <= '9'))
		{
			printf("Error only numbers must be passed as arguments.\n");
			return (0);
		}
		i++;
	}
	return (1);
}

static int	parse_arguments(int ac, char **av)
{
	int		i;
	long	nb;

	i = 0;
	while (++i < ac)
	{
		if (!is_all_numbers(av[i]))
			return (0);
		nb = ft_atol(av[i]);
		if (nb > (long)2147483647 || nb < (long)-2147483648)
		{
			printf("Error INT_MAX is the largest number accepted.\n");
			return (0);
		}
	}
	return (1);
}

int	parse(int ac, char **av, t_table *table)
{
	if (parse_arguments(ac, av) == 0)
		return (0);
	table->philo_nbr = ft_atoi(av[1]);
	table->time_to_die = (long long)ft_atoi(av[2]);
	table->time_to_eat = (long long)ft_atoi(av[3]);
	table->time_to_sleep = (long long)ft_atoi(av[4]);
	if (av[5])
		table->max_meals = ft_atoi(av[5]);
	else
		table->max_meals = -1;
	return (1);
}
