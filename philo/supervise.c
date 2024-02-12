/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/12 05:58:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_meals(t_data *data, int i)
{
	long	num;

	pthread_mutex_lock(&data->philo[i].lock_eating);
	num = data->philo[i].times_ate;
	pthread_mutex_unlock(&data->philo[i].lock_eating);
	if (num == data->notepme && data->philo[i].is_done == false)
	{
		data->philo[i].is_done = true;
		pthread_mutex_lock(&data->lock_done);
		(data->philos_done)++;
		pthread_mutex_unlock(&data->lock_done);
	}
}

void	*supervisor_routine(void *arg)
{
	t_data		*data;
	int			i;
	long long	death_time;

	data = (t_data *)arg;
	i = 0;
	while (reached_the_end(data) == false)
	{
		pthread_mutex_lock(&data->philo[i].lock_eating);
		death_time = data->philo[i].t_of_death;
		pthread_mutex_unlock(&data->philo[i].lock_eating);
		if (current_mtime() >= death_time)
		{
			ft_print_action(&data->philo[i], is_dead);
			pthread_mutex_lock(&data->lock_dead);
			data->any_dead = true;
			pthread_mutex_unlock(&data->lock_dead);
			return (NULL);
		}
		check_meals(data, i);
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
