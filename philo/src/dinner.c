/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:59:56 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:23:09 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	monitor_death_meals(t_table *table, int i, long long time_elapsed,
		int *finished_dinner)
{
	while ((++i < table->philo_nbr) && table->philos[i].is_alive)
	{
		pthread_mutex_lock(&table->meal_mutex);
		time_elapsed = timez(&table->philos[i]) - table->philos[i].last_meal;
		pthread_mutex_unlock(&table->meal_mutex);
		if (!table->philos[i].is_full && (time_elapsed > table->time_to_die))
		{
			pthread_mutex_lock(&table->state_mutex);
			table->philos[i].is_alive = table->end++;
			pthread_mutex_unlock(&table->state_mutex);
			pthread_mutex_lock(&table->print_mutex);
			printf("%lld %d died\n", timez(&table->philos[i]),
				table->philos[i].id);
			return (pthread_mutex_unlock(&table->print_mutex), (void)0);
		}
		pthread_mutex_lock(&table->meal_mutex);
		if (table->max_meals > 0 && table->philos[i].count == table->max_meals)
			(*finished_dinner)++;
		pthread_mutex_unlock(&table->meal_mutex);
	}
}

static void	*monitor(t_table *table)
{
	int			i;
	long long	time_elapsed;
	int			finished_dinner;

	while (1)
	{
		i = -1;
		finished_dinner = 0;
		time_elapsed = 0;
		pthread_mutex_lock(&table->state_mutex);
		if (table->end)
			return (pthread_mutex_unlock(&table->state_mutex), NULL);
		pthread_mutex_unlock(&table->state_mutex);
		monitor_death_meals(table, i, time_elapsed, &finished_dinner);
		if (finished_dinner == table->philo_nbr)
		{
			pthread_mutex_lock(&table->state_mutex);
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
		}
		if (table->end)
			return (NULL);
		my_sleep(1);
	}
}

static void	handle_thread(t_table *table, pthread_t monitor_th)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philosopher_routine,
				&table->philos[i]) != 0)
		{
			printf("Failed to create thread for philosopher %d.\n", i);
			pthread_mutex_lock(&table->state_mutex);
			table->end = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return ;
		}
	}
	i = -1;
	while (++i < table->philo_nbr)
		pthread_join(table->philos[i].thread, NULL);
	pthread_join(monitor_th, NULL);
}

void	dinner(void *arg)
{
	pthread_t	monitor_th;
	t_table		*table;

	table = (t_table *)arg;
	if (table->philo_nbr == 0)
	{
		printf("Error the number of philosophers must be at least one.\n");
		return ;
	}
	if (table->philo_nbr == 1)
	{
		handle_one_philo(table);
		return ;
	}
	if (pthread_create(&monitor_th, NULL, (void *)monitor, (void *)table) != 0)
	{
		printf("Failed to create monitor.\n");
		return ;
	}
	if (table->max_meals == 0)
	{
		printf("Error the numbers of meals must be at least one.\n");
		return ;
	}
	handle_thread(table, monitor_th);
}
