/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:29 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:17:06 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	init_forks(t_philo *philo, t_fork *forks, int pos)
{
	int	nbr;

	nbr = philo->table->philo_nbr;
	if (philo->id % 2 == 0)
	{
		philo->right_fork = &forks[pos];
		philo->left_fork = &forks[(pos + 1) % nbr];
	}
	else
	{
		philo->right_fork = &forks[(pos + 1) % nbr];
		philo->left_fork = &forks[pos];
	}
}

static void	init_philosophers(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->table = table;
		philo->last_meal = timez(philo);
		philo->is_full = 0;
		philo->is_alive = 1;
		philo->count = 0;
		init_forks(philo, table->forks, i);
	}
}

void	init_table(t_table *table)
{
	int	i;

	table->end = 0;
	table->start = current_time_ms();
	pthread_mutex_init(&table->state_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->meal_mutex, NULL);
	pthread_mutex_init(&table->forks_mutex, NULL);
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
		return ;
	table->forks = (t_fork *)malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
	{
		free(table->philos);
		return ;
	}
	i = -1;
	while (++i < table->philo_nbr)
	{
		pthread_mutex_init(&table->forks[i].fork, NULL);
		table->forks[i].fork_id = i;
		table->forks[i].lock = 0;
	}
	init_philosophers(table);
}
