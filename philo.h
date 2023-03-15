/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:10:22 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/15 16:00:19 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_info
{
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	int				times_to_eat;
	int				any_dead;
	int				reached_limit;
	pthread_t		*threads;
	long int		start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	instruction;
	pthread_mutex_t	n_eats;
	pthread_mutex_t	l_eat;
	pthread_mutex_t	crit;
	pthread_mutex_t	init;
}					t_info;

typedef struct s_philo
{
	long int		last_eat;
	int				nbr;
	int				nbr_eats;
	t_info			*info;
}					t_philo;

int					ft_atoi(const char *str);
void				*ft_calloc(size_t nitems, size_t size);
int					init_process(t_philo *philos, t_info *info);
int					eating_sleeping(t_philo *philo);
int					thinking(t_philo *philo);
void				take_forks(t_philo *philo);
int					philo_dead(t_philo *philo);
long int			calc_time(void);
void				print_instruction(t_philo *philo, long int now, char c);
void				ft_usleep(int ms);
int					kill_threads(t_philo *philo);
#endif
