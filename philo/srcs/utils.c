/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 21:05:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/09 15:56:31 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long long	ft_atoll(const char *str)
{
	long long	num;

	num = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	return (num);
}

long long	current_mtime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_msleep(long long msec)
{
	long long	start;

	start = current_mtime();
	while ((current_mtime() - start) < msec)
		usleep(100); //! i changed that. it was 200 before. check which one is better
}

bool	ft_exit(char *msg, t_philo *philo, int num, bool join)
{
	int	i;

	if (msg != NULL)
		printf(ERROR "%s\n", msg);
	i = -1;
	if (join == true) //! i deleted all the brackets inside here:
	{
		while (++i < num)
			if (philo[i].thread)
				if (pthread_join(philo[i].thread, NULL) != 0)
					return (false);
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
	long long	now;

	pthread_mutex_lock(&philo->data->lock_print);
	if (no_philo_dead(philo))
	{
		now = current_mtime() - philo->data->start_time;
		if (action == takes_fork)
			printf("%lld %d %s\n", now, philo->id, "has taken a fork");
		else if (action == eats)
			printf("%lld %d %s\n", now, philo->id, "is eating");
		else if (action == sleeps)
			printf("%lld %d %s\n", now, philo->id, "is sleeping");
		else if (action == thinks)
			printf("%lld %d %s\n", now, philo->id, "is thinking");
		else
			printf("%lld %d %s\n", now, philo->id, "died");
		pthread_mutex_unlock(&philo->data->lock_print);
		return ;
	}
	pthread_mutex_unlock(&philo->data->lock_print);
}
