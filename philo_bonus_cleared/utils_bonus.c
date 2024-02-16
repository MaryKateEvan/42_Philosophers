/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 21:05:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 11:44:34 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		usleep(200);
}

//! will only exit with a positive exit_code value
void	ft_exit(char *msg, t_data *data, int num, int exit_code)
{
	if (msg != NULL)
		printf(ERROR "%s\n", msg);
	if (data != NULL)
	{
		sem_close(data->forks);
		sem_unlink("/forks");
		sem_close(data->sem_dead);
		sem_unlink("/dead");
		sem_close(data->sem_print);
		sem_unlink("/print");
		while (data->philo != NULL && num--)
		{
			sem_close(data->philo[num].lock_time);
			sem_unlink(data->philo[num].time_name);
			free(data->philo[num].time_name);
			sem_close(data->philo[num].sem_is_done);
			sem_unlink(data->philo[num].done_name);
			free(data->philo[num].done_name);
		}
		if (data->philo)
			free(data->philo);
	}
	exit(exit_code);
}

void	ft_print_action(t_philo *philo, t_state action)
{
	long long	now;

	sem_wait(philo->data->sem_print);
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
	{
		printf("%lld %d %s\n", now, philo->id, "died");
		sem_post(philo->data->sem_dead);
		return ;
	}
	sem_post(philo->data->sem_print);
}
