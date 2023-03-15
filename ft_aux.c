/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 19:57:45 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 19:52:34 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Al instructions will be printed to terminal dependind on char passed
only 1 instruction can be printed at the same time because of mutexes.
When philo is eating is printed last eat time is updated.*/
void	print_instruction(t_philo *philo, long int now, char c)
{
	long int	delta;

	delta = now - philo->info->start;
	pthread_mutex_lock(&philo->info->instruction);
	pthread_mutex_lock(&philo->info->crit);
	if (philo->info->reached_limit != 1 && philo->info->any_dead != 1)
	{
		if (c == 'f')
			printf("%ld %d has taken a fork\n", delta, philo->nbr);
		else if (c == 'e')
		{
			printf("%ld %d is eating\n", delta, philo->nbr);
			pthread_mutex_lock(&philo->info->l_eat);
			philo->last_eat = now;
			pthread_mutex_unlock(&philo->info->l_eat);
		}
		else if (c == 's')
			printf("%ld %d is sleeping\n", delta, philo->nbr);
		else if (c == 't')
			printf("%ld %d is thinking\n", delta, philo->nbr);
		else if (c == 'd')
			printf("%ld %d died\n", delta, philo->nbr);
	}
	pthread_mutex_unlock(&philo->info->crit);
	pthread_mutex_unlock(&philo->info->instruction);
}

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*temp;

	temp = (void *)malloc(size * nitems);
	if (!temp)
		return (NULL);
	memset(temp, 0, size);
	return (temp);
}

int	ft_atoi(const char *str)
{
	int	sinal;
	int	res;
	int	i;

	sinal = 1;
	res = 0;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sinal = -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (sinal * res);
}

/*Calculate current time using gettimeofday function
seconds * 1000 + useconds / 1000 = time in ms - microseconds*/
long int	calc_time(void)
{
	struct timeval	now;
	long int		time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec * 1000 + now.tv_usec / 1000);
	return (time);
}

/*sleep x microseconds*/
void	ft_usleep(int ms)
{
	usleep(ms * 1000);
}
