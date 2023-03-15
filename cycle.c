/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:30:32 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 19:49:12 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Simulates philo eating and sleeping.
Print eating instruction and sleeps [time_to_eat]].
Print sleeping instruction and then unlock forks.
After that philo will sleep [time to sleep]
and increments nbr_eats of philo*/
int	eating_sleeping(t_philo *philo)
{
	print_instruction(philo, calc_time(), 'e');
	ft_usleep(philo->info->time_to_eat);
	print_instruction(philo, calc_time(), 's');
	if (philo->nbr == 1)
	{
		pthread_mutex_unlock(&philo->info->forks[philo->info->nbr_philo - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
	}
	else
	{
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 2]);
	}
	ft_usleep(philo->info->time_to_sleep);
	pthread_mutex_lock(&philo->info->n_eats);
	philo->nbr_eats++;
	pthread_mutex_unlock(&philo->info->n_eats);
	return (1);
}

/*Fork mutex is locked for the inex passed
and instruction of fork being taken is printed*/
int	lock_fork(t_philo *philo, int fork_index)
{
	pthread_mutex_lock(&philo->info->forks[fork_index]);
	print_instruction(philo, calc_time(), 'f');
	return (1);
}

/*All philos last are initialized (=start of simulation).
Each philo has a fork (the left fork) and 
will use the fork from other philo to eat (right fork).
To eat each philo will lock left fork (lowest index) 
and lock right fork (highest index)by this order. 
In the case of philo nbr 1 the right fork is the fork of the last philo*/
void	take_forks(t_philo *philo)
{
	int			i;
	static int	start;

	i = -1;
	pthread_mutex_lock(&philo->info->init);
	if (start == 0)
	{
		philo->info->start = calc_time();
		while (++i < philo->info->nbr_philo)
			philo[i].last_eat = philo->info->start;
		start++;
	}
	pthread_mutex_unlock(&philo->info->init);
	if (philo->nbr == 1)
		lock_fork(philo, philo->nbr - 1);
	else
		lock_fork(philo, philo->nbr - 2);
	if (philo->nbr == 1)
		lock_fork(philo, philo->info->nbr_philo - 1);
	else
		lock_fork(philo, philo->nbr - 1);
}

/*This function will check if any philo is dead.
The last eat time (that is being protected by mutex) is
compared to [time to die]
if now time - last eat time > time to die philo will die
and instruction is printed and flag dead will be equal to 1*/
int	philo_dead(t_philo *philo)
{
	int			i;
	long int	last_eat;

	i = -1;
	while (++i < philo->info->nbr_philo)
	{
		pthread_mutex_lock(&philo->info->l_eat);
		last_eat = philo[i].last_eat;
		pthread_mutex_unlock(&philo->info->l_eat);
		if ((calc_time() - last_eat) < philo[i].info->time_to_die)
			return (0);
		print_instruction(&philo[i], calc_time(), 'd');
		pthread_mutex_lock(&philo->info->crit);
		philo->info->any_dead = 1;
		pthread_mutex_unlock(&philo->info->crit);
		return (1);
	}
	return (0);
}

/*All variables that can be changed by other threads are mutex protected
If nbr eats limit is passed this function will check if all philos
have already eat [limit] times and if so will put
flag reached limit = 1 that will be used by main proccess to end program*/
int	check_philo_eats(t_philo *philo)
{
	int	i;
	int	nbr_eats;

	i = 0;
	if (philo[i].info->times_to_eat == -1)
		return (0);
	while (i < philo->info->nbr_philo)
	{
		pthread_mutex_lock(&philo->info->n_eats);
		nbr_eats = philo[i].nbr_eats;
		pthread_mutex_unlock(&philo->info->n_eats);
		if (nbr_eats < philo[i].info->times_to_eat)
			return (0);
		i++;
	}
	pthread_mutex_lock(&philo->info->crit);
	philo->info->reached_limit = 1;
	pthread_mutex_unlock(&philo->info->crit);
	return (1);
}
