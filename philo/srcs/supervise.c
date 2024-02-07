/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/07 07:00:18 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

void	*supervisor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	i = 0;
	while (reached_the_end(data) == false)
	{
		pthread_mutex_lock(&data->philo[i].lock_eating);
		if (current_mtime() >= data->philo[i].t_of_death
			&& data->philo[i].is_eating == false)
		{
			ft_print_action(&data->philo[i], is_dead);
			pthread_mutex_lock(&data->lock_dead);
			data->any_dead = true;
			pthread_mutex_unlock(&data->lock_dead);
			return (pthread_mutex_unlock(&data->philo[i].lock_eating), NULL);
		}
		pthread_mutex_unlock(&data->philo[i].lock_eating);
		if (++i == data->num_philos)
			i = 0;
	}
	return (NULL);
}
