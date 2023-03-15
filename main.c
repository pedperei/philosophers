/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:18:00 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 16:56:11 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_info(t_info *info, t_philo *philos)
{
	int	i;

	pthread_mutex_destroy(&info->instruction);
	pthread_mutex_destroy(&info->crit);
	pthread_mutex_destroy(&info->init);
	pthread_mutex_destroy(&info->n_eats);
	pthread_mutex_destroy(&info->l_eat);
	i = 0;
	while (i < info->nbr_philo)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(info->forks);
	free(info->threads);
	free(info->forks);
	free(info);
	free(philos);
	return (0);
}

t_info	*init_info(char **argv, int argc)
{
	t_info	*info;

	info = (t_info *)ft_calloc(1, sizeof(t_info));
	if (!info)
		return (0);
	info->nbr_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->times_to_eat = ft_atoi(argv[5]);
	else
		info->times_to_eat = -1;
	info->any_dead = 0;
	info->reached_limit = 0;
	info->start = -1;
	pthread_mutex_init(&info->instruction, NULL);
	pthread_mutex_init(&info->n_eats, NULL);
	pthread_mutex_init(&info->l_eat, NULL);
	pthread_mutex_init(&info->crit, NULL);
	pthread_mutex_init(&info->init, NULL);
	info->threads = (pthread_t *)malloc((info->nbr_philo) * sizeof(pthread_t));
	if (!info->threads)
		return (0);
	return (info);
}

t_philo	*init_philos_mutex(t_info *info)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)ft_calloc(info->nbr_philo, sizeof(t_philo));
	info->forks = (pthread_mutex_t *)ft_calloc(info->nbr_philo,
			sizeof(pthread_mutex_t));
	if (!philos || !info->forks)
		return (0);
	i = 0;
	while (i < info->nbr_philo)
	{
		philos[i].info = info;
		philos[i].nbr = i + 1;
		philos[i].nbr_eats = 0;
		if (pthread_mutex_init(&philos[i].info->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	return (philos);
}

int	join_threads(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_join(info->threads[i], NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_info	*info;

	if (argc != 5 && argc != 6)
	{
		printf("Invalid number of args");
		return (0);
	}
	else
	{
		info = init_info(argv, argc);
		philos = init_philos_mutex(info);
		if (!philos || !info)
			return (free_info(info, philos));
		if (!init_process(philos, info))
			return (free_info(info, philos));
		while (1)
		{
			if (info->reached_limit == 1 || info->any_dead == 1)
				break ;
		}
		join_threads(info);
	}
	free_info(info, philos);
}
