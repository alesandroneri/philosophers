/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:40 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:08:07 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	get_fork(philo, philo->right_fork);
	my_sleep(philo->table->time_to_die);
	put_fork(philo, philo->right_fork);
	philo->is_alive = 0;
	philo->table->end = 1;
	printf("%lld %d died\n", timez(philo), philo->id);
	return (NULL);
}

void	handle_one_philo(t_table *table)
{
	if (pthread_create(&table->philos[0].thread, NULL, one_philo,
			&table->philos[0]) != 0)
	{
		printf("Failed to create thread for philosopher 1.\n");
		return ;
	}
	pthread_join(table->philos[0].thread, NULL);
}

void	destroy_mutex(t_table *table)
{
	pthread_mutex_destroy(&table->state_mutex);
	pthread_mutex_destroy(&table->meal_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->forks_mutex);
}

void	free_resources(t_table *table)
{
	int	i;

	if (!table)
		return ;
	if (table->forks)
	{
		i = -1;
		while (++i < table->philo_nbr)
		{
			pthread_mutex_lock(&table->forks_mutex);
			if (table->forks[i].lock == 1)
			{
				table->forks[i].lock = 0;
				pthread_mutex_unlock(&table->forks[i].fork);
				pthread_mutex_destroy(&table->forks[i].fork);
			}
			else if (table->forks[i].lock == 0)
				pthread_mutex_destroy(&table->forks[i].fork);
			pthread_mutex_unlock(&table->forks_mutex);
		}
		free(table->forks);
	}
	destroy_mutex(table);
	if (table->philos)
		free(table->philos);
}
