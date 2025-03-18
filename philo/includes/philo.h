/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneri-da <aneri-da@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:07:12 by aneri-da          #+#    #+#             */
/*   Updated: 2025/03/18 20:29:54 by aneri-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_fork
{
	pthread_mutex_t		fork;
	int					fork_id;
	int					lock;
}						t_fork;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	t_fork				*right_fork;
	t_fork				*left_fork;
	int					count;
	long long			last_meal;
	int					is_full;
	int					is_alive;
	t_table				*table;
}						t_philo;

struct					s_table
{
	int					philo_nbr;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					max_meals;
	long long			start;
	int					end;
	t_fork				*forks;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		state_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		forks_mutex;
	t_philo				*philos;
} ;

long long				timez(t_philo *philo);
int						parse(int ac, char **av, t_table *table);
void					init_table(t_table *table);
long long				current_time_ms(void);
void					get_time(long long milisecond);
void					free_resources(t_table *table);
void					get_fork(t_philo *philo, t_fork *fork);
void					put_fork(t_philo *philo, t_fork *fork);
void					dinner(void *arg);
void					grab_forks(t_philo *philo, t_fork *fork_one,
							t_fork *fork_two);
void					leave_forks(t_philo *philo, t_fork *fork_one,
							t_fork *fork_two);
void					my_sleep(long long sleeping);
void					handle_one_philo(t_table *table);
void					*philosopher_routine(void *arg);

#endif