/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/30 20:00:42 by mevangel         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->lock_philo); // i need the mutex so that the "supervisor" isn't accesing the is_eating and t_until_death at the same time
	philo->is_eating = true;
	ft_print_action(philo, eats);
	philo->t_until_death = current_mtime() + philo->data->t_die; //? do i need that?
	philo->times_ate++;
	// if (philo->times_ate == philo->data->notepme)
	// 	philo->finished = true; //? do i actually need both these two variables or only one?
	pthread_mutex_unlock(&philo->lock_philo);
	ft_msleep(philo->data->t_eat);
	pthread_mutex_unlock(&philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	philo->is_eating = false;
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *) arg;
	// printf("hello from thread %d\n", philo->id);
	philo->t_until_death = current_mtime() + philo->data->t_die;
	// ? maybe a supervisor thread creation here? 
	if (philo->data->any_dead == false)
	{
		ft_print_action(philo, thinks);
		ft_eat(philo);
		ft_print_action(philo, sleeps);
		ft_msleep(philo->data->t_sleep);
	}
	return (NULL);
}

