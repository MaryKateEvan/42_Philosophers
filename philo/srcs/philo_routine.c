/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/09 20:08:35 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	one_philo_routine(t_philo *philo);
static void	ft_eat(t_philo *philo);
static void	ft_take_forks(t_philo *philo);
static void	ft_leave_forks(t_philo *philo);

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
	while (reached_the_end(philo->data) == false)
	{
		ft_eat(philo);
		if (reached_the_end(philo->data))
			break ;
		ft_print_action(philo, sleeps);
		ft_msleep(philo->data->t_sleep);
		if (reached_the_end(philo->data))
			break ;
		ft_print_action(philo, thinks);
	}
	return (NULL);
}

static void	one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->r_fork);
	ft_print_action(philo, takes_fork);
	pthread_mutex_lock(&philo->lock_eating);
	philo->t_of_death = current_mtime() + philo->data->t_die;
	pthread_mutex_unlock(&philo->lock_eating);
	pthread_mutex_unlock(&philo->r_fork);
}

static void	ft_eat(t_philo *philo)
{
	ft_take_forks(philo);
	pthread_mutex_lock(&philo->lock_eating);
	philo->t_of_death = current_mtime() + philo->data->t_die;
	pthread_mutex_unlock(&philo->lock_eating);
	ft_print_action(philo, eats);
	ft_msleep(philo->data->t_eat);
	ft_leave_forks(philo);
	if (philo->data->notepme > 0)
	{
		pthread_mutex_lock(&philo->lock_eating);
		(philo->times_ate)++;
		pthread_mutex_unlock(&philo->lock_eating);
	}
}

static void	ft_take_forks(t_philo *philo)
{
	if ((philo->id % 2 != 0))
	{
		usleep(300);
		pthread_mutex_lock(&philo->r_fork);
		ft_print_action(philo, takes_fork);
		pthread_mutex_lock(philo->l_fork);
		ft_print_action(philo, takes_fork);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		ft_print_action(philo, takes_fork);
		pthread_mutex_lock(&philo->r_fork);
		ft_print_action(philo, takes_fork);
	}
}

static void	ft_leave_forks(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(&philo->r_fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}
