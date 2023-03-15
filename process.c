/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 16:29:37 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		thinking(phil);
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

void	*one_philo(void *arg)
{
	t_philo	*phil;
	long	start;
	int		i;

	phil = (t_philo *)arg;
	pthread_mutex_lock(&phil->info->init);
	start = phil->info->start;
	pthread_mutex_unlock(&phil->info->init);
	i = 0;
	if (start == -1)
	{
		pthread_mutex_lock(&phil->info->init);
		pthread_mutex_lock(&phil->info->l_eat);
		phil->info->start = calc_time();
		phil[i].last_eat = phil->info->start;
		pthread_mutex_unlock(&phil->info->l_eat);
		pthread_mutex_unlock(&phil->info->init);
	}
	print_instruction(phil, calc_time(), 'f');
	ft_usleep(phil->info->time_to_die);
	print_instruction(phil, calc_time(), 'd');
	return (0);
}

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
