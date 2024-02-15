/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:12:32 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/15 04:38:56 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_meals(t_data *data, int i)
{
	long	num;

	sem_wait(data->philo[i].sem_eating);
	num = data->philo[i].times_ate;
	sem_post(data->philo[i].sem_eating);
	if (num == data->notepme && data->philo[i].is_done == false)
	{
		data->philo[i].is_done = true;
		sem_wait(data->sem_done);
		(data->philos_done)++;
		sem_post(data->sem_done);
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
		sem_wait(data->philo[i].sem_eating);
		death_time = data->philo[i].t_of_death;
		sem_post(data->philo[i].sem_eating);
		if (current_mtime() >= death_time)
		{
			ft_print_action(&data->philo[i], is_dead, data);
			sem_wait(data->sem_dead);
			data->any_dead = true;
			sem_post(data->sem_dead);
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

bool	no_philo_dead(t_data *data)
{
	bool	check;

	sem_wait(data->sem_dead);
	check = data->any_dead;
	sem_post(data->sem_dead);
	return (!check);
}
