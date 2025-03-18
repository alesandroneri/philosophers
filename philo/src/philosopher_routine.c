/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:06 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:10:24 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	grab_forks(philo, philo->right_fork, philo->left_fork);
	pthread_mutex_lock(&philo->table->meal_mutex);
	philo->last_meal = timez(philo);
	pthread_mutex_unlock(&philo->table->meal_mutex);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
	{
		pthread_mutex_lock(&philo->table->print_mutex);
		printf("%lld %d is eating\n", philo->last_meal, philo->id);
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	my_sleep(philo->table->time_to_eat);
	leave_forks(philo, philo->right_fork, philo->left_fork);
	pthread_mutex_lock(&philo->table->meal_mutex);
	if (++philo->count == philo->table->max_meals)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->table->meal_mutex);
}

static void	to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
	{
		pthread_mutex_lock(&philo->table->print_mutex);
		printf("%lld %d is sleeping\n", timez(philo), philo->id);
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	my_sleep(philo->table->time_to_sleep);
}

static void	to_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
	{
		pthread_mutex_lock(&philo->table->print_mutex);
		printf("%lld %d is thinking\n", timez(philo), philo->id);
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
}

static void	unlock_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->forks_mutex);
	if (philo->right_fork->lock == 1)
	{
		pthread_mutex_unlock(&philo->right_fork->fork);
		philo->right_fork->lock = 0;
	}
	if (philo->left_fork->lock == 1)
	{
		pthread_mutex_unlock(&philo->left_fork->fork);
		philo->left_fork->lock = 0;
	}
	pthread_mutex_unlock(&philo->table->forks_mutex);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->table->state_mutex);
		if (philo->table->end)
		{
			pthread_mutex_unlock(&philo->table->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->state_mutex);
		if (philo->is_alive && (philo->table->max_meals == -1
				|| philo->count < philo->table->max_meals))
		{
			to_eat(philo);
			to_sleep(philo);
			to_think(philo);
		}
		usleep(1);
	}
	unlock_forks(philo);
	return (NULL);
}
