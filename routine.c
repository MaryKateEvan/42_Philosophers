/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/30 05:08:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_eat(t_philo *philo)
{
	//! Should I check first if t_until_death is reached?
	//? First for the eating: 
	pthread_mutex_lock(&philo->r_fork);
	ft_print_action(philo, takes_fork);
	pthread_mutex_lock(philo->l_fork);
	ft_print_action(philo, takes_fork);
	//! Should i put the odd and even philos to take the forks in reverse order?
	// philo->started_eat = current_mtime(); //? do i need that?
	ft_print_action(philo, eats);
	philo->t_until_death = current_mtime() + philo->data->t_die; //? do i need that?
	philo->times_ate++;
	if (philo->times_ate == philo->data->notepme)
	{
		philo->finished = true; //? do i actually need both these two variables or only one?
		pthread_mutex_lock(&philo->data->done);
		philo->data->philos_done++;
		pthread_mutex_unlock(&philo->data->done);
	}
	pthread_mutex_unlock(&philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	ft_msleep(philo->data->t_eat);
}

void	ft_actions(t_philo *philo, bool *stop)
{
	if (current_mtime() > philo->t_until_death)
	{
		pthread_mutex_lock(&philo->data->dead);
		philo->data->any_dead = true;
		pthread_mutex_unlock(&philo->data->dead);
		ft_print_action(philo, is_dead);
		*stop = true;
		return ;
	}
	if (philo->data->philos_done >= philo->data->num_philos)
	{
		ft_print_action(philo, finished);
		*stop = true;
		return ;
	}
	ft_eat(philo);
	ft_print_action(philo, sleeps);
	ft_msleep(philo->data->t_sleep);
	ft_print_action(philo, thinks);
}



void	*philo_routine(void *arg)
{
	t_philo			*philo;
	// unsigned long	time_now;
	bool			stop;

	philo = (t_philo *) arg;
	printf("hello from thread %d\n", philo->id);
	// philo->t_until_death = current_mtime() + (unsigned long)philo->data->t_die;
	// ? maybe a supervisor thread creation here? 
	stop = false;
	if ((philo->id % 2) == 0)
	{
		ft_print_action(philo, thinks);
		ft_msleep(2);
	}
	// while (stop == false)
	// 	ft_actions(philo, &stop);
	return (NULL);
}