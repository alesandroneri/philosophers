/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:37 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:07:38 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
