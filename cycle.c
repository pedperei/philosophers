/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:30:32 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 16:24:00 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	thinking(t_philo *philo)
{
	print_instruction(philo, calc_time(), 't');
	ft_usleep(philo->info->time_to_think);
	return (1);
}

int	lock_fork(t_philo *philo, int fork_index)
{
	pthread_mutex_lock(&philo->info->forks[fork_index]);
	print_instruction(philo, calc_time(), 'f');
	return (1);
}

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
