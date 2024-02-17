/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/17 12:50:56 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	one_philo_routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	ft_print_action(philo, takes_fork);
	sem_wait(philo->lock_time);
	philo->t_of_death = current_mtime() + philo->data->t_die;
	sem_post(philo->lock_time);
	sem_post(philo->data->forks);
}

static void	ft_eat(t_philo *philo)
{
	if ((philo->id % 2 != 0))
		usleep(300);
	sem_wait(philo->data->forks);
	ft_print_action(philo, takes_fork);
	sem_wait(philo->data->forks);
	ft_print_action(philo, takes_fork);
	sem_wait(philo->lock_time);
	philo->t_of_death = current_mtime() + philo->data->t_die;
	sem_post(philo->lock_time);
	ft_print_action(philo, eats);
	ft_msleep(philo->data->t_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	if (philo->data->notepme > 0)
	{
		if (++(philo->times_ate) == philo->data->notepme)
			sem_post(philo->sem_is_done);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
		return (one_philo_routine(philo), NULL);
	if (philo->id % 2 != 0)
	{
		ft_print_action(philo, thinks);
		ft_msleep(philo->data->t_eat / 2);
	}
	while (1)
	{
		ft_eat(philo);
		ft_print_action(philo, sleeps);
		ft_msleep(philo->data->t_sleep);
		ft_print_action(philo, thinks);
	}
	return (NULL);
}

void	*supervisor_routine(void *arg)
{
	t_philo		*philo;
	long long	death_time;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->lock_time);
		death_time = philo->t_of_death;
		sem_post(philo->lock_time);
		if (current_mtime() >= death_time)
		{
			ft_print_action(philo, is_dead);
			return (NULL);
		}
	}
	return (NULL);
}
