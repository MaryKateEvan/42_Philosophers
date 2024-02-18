/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 21:05:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/18 02:26:31 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	check_input(int argc, char **argv)
{
	int	i;

	if (!(argc == 5 || argc == 6))
		ft_exit("expected 5 or 6 arguments.", NULL, 0, 0);
	while (*argv)
	{
		i = 0;
		while ((*argv)[i])
		{
			if ((*argv)[i] < '0' || (*argv)[i] > '9')
				ft_exit("only positive numbers accepted", NULL, 0, 0);
			i++;
		}
		if (!(*argv)[0])
			ft_exit("empty argument(s)", NULL, 0, 0);
		if ((*argv)[0] == '0' && (*argv)[1] == '\0')
			ft_exit("all arguments must be greater than zero", NULL, 0, 0);
		argv++;
	}
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
		usleep(200);
}

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
		while (num--)
		{
			sem_unlink(data->philo[num].time_name);
			sem_close(data->philo[num].lock_time);
			free(data->philo[num].time_name);
			sem_unlink(data->philo[num].done_name);
			sem_close(data->philo[num].sem_is_done);
			free(data->philo[num].done_name);
		}
		if (data->philo)
			free(data->philo);
		if (data->pid_array)
			free(data->pid_array);
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
