/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/10 20:48:25 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_for_dead(t_philo *philo)
{
		pthread_mutex_lock(&philo->data->lock_time);
		if (current_mtime() >= philo->t_of_death)
		{
			pthread_mutex_unlock(&data->lock_time);
			ft_print_action(&data->philo[i], is_dead);
			pthread_mutex_lock(&data->lock_dead);
			data->any_dead = true;
			pthread_mutex_unlock(&data->lock_dead);
			return (NULL);
		}
		pthread_mutex_unlock(&data->lock_time);
}

void	check_for_meals(t_philo *philo)
{
	
}

void	*supervisor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	times_ate_tmp;

	data = (t_data *)arg;
	i = 0;
	while (reached_the_end(data) == false)
	{
		check_for_dead(&data->philo[i]);
		
		if (data->philo[i].times_ate == data->notepme && !data->philo[i].is_full)
		{
			data->philo[i].is_full = true;
			pthread_mutex_lock(&data->lock_done);
			(data->philos_done)++;
			pthread_mutex_unlock(&data->lock_done);
		}
		if (++i == data->num_philos)
			i = 0;
	}
	return (NULL);
}

bool	reached_the_end(t_data *data)
{
	bool	ret;

	ret = false;
	pthread_mutex_lock(&data->lock_dead);
	if (data->any_dead == true)
		ret = true;
	pthread_mutex_unlock(&data->lock_dead);
	if (data->notepme > 0)
	{
		pthread_mutex_lock(&data->lock_done);
		if (data->philos_done == data->num_philos)
			ret = true;
		pthread_mutex_unlock(&data->lock_done);
	}
	return (ret);
}

bool	no_philo_dead(t_philo *philo)
{
	bool	check;

	pthread_mutex_lock(&philo->data->lock_dead);
	check = philo->data->any_dead;
	pthread_mutex_unlock(&philo->data->lock_dead);
	return (!check);
}
