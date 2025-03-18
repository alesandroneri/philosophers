/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:33 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:09:16 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	get_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&fork->fork);
	pthread_mutex_lock(&philo->table->forks_mutex);
	if (fork->lock == 0)
		fork->lock = 1;
	pthread_mutex_unlock(&philo->table->forks_mutex);
	pthread_mutex_lock(&philo->table->state_mutex);
	if (!philo->table->end)
	{
		pthread_mutex_lock(&philo->table->print_mutex);
		printf("%lld %d has taken a fork\n", timez(philo), philo->id);
		pthread_mutex_unlock(&philo->table->print_mutex);
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
}

void	put_fork(t_philo *philo, t_fork *fork)
{
	(void)philo;
	pthread_mutex_lock(&philo->table->forks_mutex);
	if (fork->lock == 1)
		pthread_mutex_unlock(&fork->fork);
	fork->lock = 0;
	pthread_mutex_unlock(&philo->table->forks_mutex);
}

void	grab_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	get_fork(philo, fork_one);
	get_fork(philo, fork_two);
}

void	leave_forks(t_philo *philo, t_fork *fork_one, t_fork *fork_two)
{
	pthread_mutex_lock(&philo->table->state_mutex);
	if (philo->table->end || !philo->is_alive || philo->is_full)
	{
		pthread_mutex_unlock(&philo->table->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->state_mutex);
	put_fork(philo, fork_one);
	put_fork(philo, fork_two);
}
