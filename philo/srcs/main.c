/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/07 07:04:36 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

bool	ft_input_is_valid(int argc, char **argv)
{
	int	i;

	if (!(argc == 5 || argc == 6))
		return (false);
	while (*argv)
	{
		i = 0;
		while ((*argv)[i])
		{
			if ((*argv)[i] < '0' || (*argv)[i] > '9')
				return (false);
			i++;
		}
		if (!(*argv)[0])
			return (false);
		if ((*argv)[0] == '0' && (*argv)[1] == '\0')
			return (false);
		argv++;
	}
	return (true);
}

bool	ft_init_sdata(int argc, char **argv, t_data *data)
{
	data->num_philos = (int)ft_atol(argv[0]);
	if (data->num_philos > 200)
		return (printf(ERROR "philos can not be more than 200.\n"), false);
	data->t_die = ft_atol(argv[1]);
	data->t_eat = ft_atol(argv[2]);
	data->t_sleep = ft_atol(argv[3]);
	data->notepme = -1;
	data->any_dead = false;
	data->done_eating = false;
	data->philos_done = 0;
	data->start_time = current_mtime();
	if (argc == 6)
		data->notepme = ft_atol(argv[4]);
	if (pthread_mutex_init(&data->lock_dead, NULL)
		|| pthread_mutex_init(&data->lock_done, NULL)
		|| pthread_mutex_init(&data->lock_print, NULL))
		return (printf(ERROR "pthread_mutex_init failed.\n"), false);
	return (true);
}

bool	ft_init_sphilo(t_data *data)
{
	int	id;

	id = 0;
	data->philo = malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philo == NULL)
		return (printf(ERROR "malloc failed.\n"), false);
	while (++id <= data->num_philos)
	{
		data->philo[id - 1].id = id;
		data->philo[id - 1].t_of_death = 0;
		data->philo[id - 1].times_ate = 0;
		data->philo[id - 1].is_eating = false;
		data->philo[id - 1].t_of_death = current_mtime() + data->t_die;
		data->philo[id - 1].data = data;
		if (pthread_mutex_init(&data->philo[id - 1].r_fork, NULL) != 0)
			return (ft_exit("mutex_init failed", data->philo, --id,
					false), false);
		if (id > 1)
			data->philo[id - 1].l_fork = &data->philo[id - 2].r_fork;
	}
	data->philo[0].l_fork = &data->philo[data->num_philos - 1].r_fork;
	if (pthread_mutex_init(&data->philo->lock_eating, NULL) != 0)
		return (ft_exit("mutex_init failed", data->philo, --id, false), false);
	return (true);
}

bool	ft_threads(t_data *data)
{
	int			i;
	pthread_t	supervisor;

	i = -1;
	while (++i < data->num_philos)
		if (pthread_create(&(data->philo[i].thread), NULL, &philo_routine,
				&data->philo[i]))
			return (ft_exit("pthread_create failed", data->philo, i,
					true), false);
	if (pthread_create(&supervisor, NULL, &supervisor_routine, data) != 0)
		return (ft_exit("pthread_create failed", data->philo, data->num_philos,
				false), false);
	if (pthread_join(supervisor, NULL) != 0)
		return (printf(ERROR "pthread_join failed\n"), false);
	if (!ft_exit(NULL, data->philo, data->num_philos, true))
		return (printf(ERROR "pthread_join failed\n"), false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!ft_input_is_valid(argc, ++argv))
		return (printf(ERROR "invalid input\n"), 0);
	if (!ft_init_sdata(argc, argv, &data))
		return (1);
	if (!ft_init_sphilo(&data))
		return (2);
	if (!ft_threads(&data))
		return (3);
	return (EXIT_SUCCESS);
}
