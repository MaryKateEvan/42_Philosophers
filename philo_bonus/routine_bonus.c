/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/15 04:05:56 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	one_philo_routine(t_philo *philo, t_data *data, sem_t *forks)
{
	sem_wait(forks);
	ft_print_action(philo, takes_fork, data);
	sem_wait(philo->sem_eating);
	philo->t_of_death = current_mtime() + data->t_die;
	sem_post(philo->sem_eating);
	sem_post(forks);
}

static void	ft_eat(t_philo *philo, t_data *data, sem_t *forks)
{
	if ((philo->id % 2 != 0))
		usleep(300);
	sem_wait(forks);
	ft_print_action(philo, takes_fork, data);
	sem_wait(forks);
	ft_print_action(philo, takes_fork, data);
	sem_wait(philo->sem_eating);
	philo->t_of_death = current_mtime() + data->t_die;
	sem_post(philo->sem_eating);
	ft_print_action(philo, eats, data);
	ft_msleep(data->t_eat);
	sem_post(forks);
	sem_post(forks);
	if (data->notepme > 0)
	{
		sem_wait(philo->sem_eating);
		(philo->times_ate)++;
		sem_post(philo->sem_eating);
	}
}

void	*philo_routine(t_philo *philo, t_data *data, sem_t *forks)
{
	if (data->num_philos == 1)
		return (one_philo_routine(philo, data, forks), NULL);
	if (philo->id % 2 != 0)
	{
		ft_print_action(philo, thinks, data);
		ft_msleep(data->t_eat / 2);
	}
	while (reached_the_end(data) == false)
	{
		ft_eat(philo, data, forks);
		if (reached_the_end(data))
			break ; //?should i change it to exit, so that i exit from the process?
		ft_print_action(philo, sleeps, data);
		ft_msleep(data->t_sleep);
		if (reached_the_end(data))
			break ;
		ft_print_action(philo, thinks, data);
	}
	return (NULL);
}
