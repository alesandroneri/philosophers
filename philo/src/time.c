/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:45 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:21:47 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	get_time(long long milisecond)
{
	long long	start;

	start = current_time_ms();
	while (current_time_ms() - start < milisecond)
		usleep(200);
}

void	my_sleep(long long sleeping)
{
	long long	start;
	long long	current;

	start = current_time_ms();
	current = start;
	while (((start + sleeping) - current) > 0)
	{
		if (((start + sleeping) - current) > 10)
			usleep(((start + sleeping) - current) * 100);
		else
			usleep(150);
		current = current_time_ms();
	}
}

long long	timez(t_philo *philo)
{
	return (current_time_ms() - philo->table->start);
}

long long	current_time_ms(void)
{
	struct timeval	time;
	long long		mili;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error gettimeofday failed.\n");
		return (1);
	}
	mili = time.tv_sec * 1000;
	mili += time.tv_usec / 1000;
	return (mili);
}
