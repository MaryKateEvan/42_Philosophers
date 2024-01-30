/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 21:05:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/30 19:15:55 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	ft_atoul(const char *str)
{
	unsigned long	num;

	num = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	return (num);
}

unsigned long	current_mtime(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((unsigned long)(time.tv_sec * 1000) + (unsigned long)(time.tv_usec / 1000));
}

void	ft_msleep(unsigned long msec)
{
	unsigned long	start;

	start = current_mtime();
	while ((current_mtime() - start) < msec)
		usleep(100);
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
			if (philo[i].thread) //do i actually need the != 0 ?
				if (pthread_join(philo[i].thread, NULL) != 0) //returns 0 in success
					return (false);
			i++;
		}
	}
	while (philo != NULL && num--)
		pthread_mutex_destroy(&philo[num].r_fork);
	free(philo);
	return (true);
}

void	ft_print_action(t_philo *philo, t_state action)
{
	unsigned long	now;
	
	pthread_mutex_lock(&philo->data->print);
	now = current_mtime() - philo->data->start_time;
	if (action == takes_fork)
		printf("%lu %d %s\n", now, philo->id,"has taken a fork");
	else if (action == eats)
		printf("%lu %d %s\n", now, philo->id, "is eating");
	else if (action == sleeps)
		printf("%lu %d %s\n", now, philo->id, "is sleeping");
	else if (action == thinks)
		printf("%lu %d %s\n", now, philo->id, "is thinking");
	else if (action == is_dead)
		printf("%lu %d %s\n", now, philo->id, "died");
	// else
	// 	printf("%lu %s\n", now, "all philos ate enough!");
	pthread_mutex_unlock(&philo->data->print);
}

//* the previous printing fucntion with the colours:
// void	ft_print_action(t_philo *phil, t_state action)
// {
// 	char	*s;

// 	s = NULL;
// 	if (action == takes_fork)
// 		s = "\033[0;34mhas taken a fork\033[0;97m";
// 	if (action == eats)
// 		s = "\033[0;36mis eating\033[0;97m";
// 	if (action == sleeps)
// 		s = "\033[0;32mis sleeping\033[0;97m";
// 	if (action == thinks)
// 		s = "\033[0;35mis thinking\033[0;97m";
// 	if (action == is_dead)
// 		s = "\033[0;31mdied\033[0;97m";
// 	if (action == finished)
// 		s = "\033[0;33mand everyone else has eaten enough\033[0;97m";
// 	printf("%lu philo %d: %s\n", current_mtime() - phil->data->start_time, phil->id, s);
// }
