/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 19:04:00 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Routine that represent philo cycle of life.
even philos(nbr philo) waits 2 ms to make sure 
1 takes forks first than 2 per example
Philos cycle: take_forks, eat, sleep, think, 
check death or limit of eats reached (necessary to terminate all threads)
if one philo dies all other threads will end 
because main proccess will wait that all threads are terminated(pthread join).
These variables are being protected by mutexes to avoid data races. 
repeat.
Philo will start thinking after eating.
When philo is thinking, he will try to cath forks as soon an possible 
so thinking time is 0 if possible. Instruction of philo thinking is printed*/
void	*routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->nbr % 2 == 0)
		ft_usleep(2);
	while (1)
	{
		take_forks(phil);
		eating_sleeping(phil);
		print_instruction(phil, calc_time(), 't');
		pthread_mutex_lock(&phil->info->crit);
		if (phil->info->reached_limit == 1 || phil->info->any_dead == 1)
		{
			pthread_mutex_unlock(&phil->info->crit);
			break ;
		}
		pthread_mutex_unlock(&phil->info->crit);
	}
	return (0);
}

/*All variables that can be changed by other threads are mutex protected
Puts last eat time that is equal to simulation start 
(calc_time return current time in ms)
print take fork instruction, sleep time_to_die and print dies instruction
because philo needs 2 forks to eat. 
Case of 1 philo is being treated with brute force*/
void	*one_philo(void *arg)
{
	t_philo	*phil;
	long	start;

	phil = (t_philo *)arg;
	pthread_mutex_lock(&phil->info->init);
	start = phil->info->start;
	pthread_mutex_unlock(&phil->info->init);
	if (start == -1)
	{
		pthread_mutex_lock(&phil->info->init);
		pthread_mutex_lock(&phil->info->l_eat);
		phil->info->start = calc_time();
		phil[0].last_eat = phil->info->start;
		pthread_mutex_unlock(&phil->info->l_eat);
		pthread_mutex_unlock(&phil->info->init);
	}
	print_instruction(phil, calc_time(), 'f');
	ft_usleep(phil->info->time_to_die);
	print_instruction(phil, calc_time(), 'd');
	return (0);
}

/*Lauches threads for each philo
Sleep 2 ms between each thread is inititalizated*/
int	create_threads(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->info->nbr_philo)
	{
		if (pthread_create(&philos->info->threads[i], NULL, &routine,
				&philos[i]) != 0)
			return (0);
		ft_usleep(2);
		i++;
	}
	return (1);
}

/*Launches threads to each philo (philo struct is passed as argument) 
(case of one philo is breing treated separately with one_philo routine)
Waits 60 ms to check if any philo is dead 
or if the limit of eats has been reached. 
Break loop if any of the conditions is matched*/
int	init_process(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	if (info->nbr_philo == 1)
	{
		if (pthread_create(&info->threads[i], NULL, &one_philo,
				&philos[i]) != 0)
			return (0);
	}
	else
	{
		if (!create_threads(philos))
			return (0);
	}
	ft_usleep(60);
	while (1)
	{
		if (philo_dead(philos) || check_philo_eats(philos))
			break ;
	}
	return (1);
}
