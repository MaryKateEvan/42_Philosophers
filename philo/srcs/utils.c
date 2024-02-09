/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 21:05:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/09 10:09:52 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	return (num);
}

time_t	current_mtime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_msleep(long msec)
{
	time_t	start;

	start = current_mtime();
	while ((current_mtime() - start) < msec)
	{
		usleep(200);
	}
}

bool	ft_exit(char *msg, t_philo *philo, int num, bool join)
{
	int	i;

	if (msg != NULL)
		printf(ERROR "%s\n", msg);
	i = 0;
	if (join == true)
	{
		while (i < num)
		{
			if (philo[i].thread)
				if (pthread_join(philo[i].thread, NULL) != 0)
					return (false);
			i++;
		}
	}
	pthread_mutex_destroy(&philo->data->lock_dead);
	pthread_mutex_destroy(&philo->data->lock_done);
	pthread_mutex_destroy(&philo->data->lock_print);
	while (philo != NULL && num--)
	{
		pthread_mutex_destroy(&philo[num].r_fork);
		pthread_mutex_destroy(&philo[num].lock_eating);
	}
	free(philo);
	return (true);
}

void	ft_print_action(t_philo *philo, t_state action)
{
	long	now;
	bool	check;

	pthread_mutex_lock(&philo->data->lock_print);
	pthread_mutex_lock(&philo->data->lock_dead);
	check = philo->data->any_dead;
	pthread_mutex_unlock(&philo->data->lock_dead);
	if (check == false)
	{
		now = current_mtime() - philo->data->start_time;
		if (action == takes_fork)
			printf("%ld %d %s\n", now, philo->id, "has taken a fork");
		else if (action == eats)
			printf("%ld %d %s\n", now, philo->id, "is eating");
		else if (action == sleeps)
			printf("%ld %d %s\n", now, philo->id, "is sleeping");
		else if (action == thinks)
			printf("%ld %d %s\n", now, philo->id, "is thinking");
		else
			printf("%ld %d %s\n", now, philo->id, "died");
		pthread_mutex_unlock(&philo->data->lock_print);
		return ;
	}
	pthread_mutex_unlock(&philo->data->lock_print);
}
