/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 05:11:30 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_meals(t_philo *philo)
{
	long	num;

	sem_wait(philo->sem_eating);
	num = philo->times_ate;
	sem_post(philo->sem_eating);
	if (num == philo->data->notepme && philo->is_done == false)
	{
		philo->is_done = true;
		// sem_wait(philo->data->sem_done);
		// (philo->data->philos_done)++;
		// sem_post(philo->data->sem_done);

			sem_wait(philo->data->sem_dead);
			// sem_wait(philo->data->sem_print);
			philo->data->any_dead = true;
			sem_post(philo->data->sem_dead);
	}
}

void	*supervisor_routine(void *arg)
{
	t_philo		*philo;
	// int			i;
	long long	death_time;

	philo = (t_philo *)arg;
	// i = 0;
	// usleep(450);
	while (reached_the_end(philo->data) == false)
	{
		sem_wait(philo->sem_eating);
		death_time = philo->t_of_death;
		sem_post(philo->sem_eating);
		if (current_mtime() >= death_time)
		{
			ft_print_action(philo, is_dead);
			sem_wait(philo->data->sem_dead);
			philo->data->any_dead = true;
			sem_post(philo->data->sem_dead);
			// exit(0);
			// printf("%lld %d %s\n", current_mtime() - philo->data->start_time, philo->id, "died");
			// sem_post(philo->data->sem_print);
			return (NULL);
		}
		check_meals(philo);
		// if (++i == data->num_philos)
		// 	i = 0;
	}
	return (NULL);
}

bool	reached_the_end(t_data *data)
{
	bool	ret;

	ret = false;
	sem_wait(data->sem_dead);
	if (data->any_dead == true)
		ret = true;
	sem_post(data->sem_dead);
	if (data->notepme > 0)
	{
		sem_wait(data->sem_done);
		if (data->philos_done == data->num_philos)
			ret = true;
		sem_post(data->sem_done);
	}
	return (ret);
}

bool	no_philo_dead(t_philo *philo)
{
	bool	check;

	sem_wait(philo->data->sem_dead);
	check = philo->data->any_dead;
	sem_post(philo->data->sem_dead);
	return (!check);
}
