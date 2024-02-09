/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/09 16:58:30 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static bool	ft_input_is_valid(int argc, char **argv);
static bool	ft_init_sdata(int argc, char **argv, t_data *data);
static bool	ft_init_sphilo(t_data *data);
static bool	ft_threads(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;

	if (!ft_input_is_valid(argc, ++argv))
		return (0);
	if (!ft_init_sdata(argc, argv, &data))
		return (1);
	if (!ft_init_sphilo(&data))
		return (2);
	if (!ft_threads(&data))
		return (3);
	return (EXIT_SUCCESS);
}

static bool	ft_input_is_valid(int argc, char **argv)
{
	int	i;

	if (!(argc == 5 || argc == 6))
		return (printf(INPUT "expected 5 or 6 arguments.\n"), false);
	while (*argv)
	{
		i = 0;
		while ((*argv)[i])
		{
			if ((*argv)[i] < '0' || (*argv)[i] > '9')
				return (printf(INPUT "non-numeric characters present.\n"),
					false);
			i++;
		}
		if (!(*argv)[0])
			return (printf(INPUT "empty argument.\n"), false);
		if ((*argv)[0] == '0' && (*argv)[1] == '\0')
			return (printf(INPUT "no argument can be zero\n"), false);
		argv++;
	}
	return (true);
}

static bool	ft_init_sdata(int argc, char **argv, t_data *data)
{
	data->num_philos = (int)ft_atoll(argv[0]);
	data->t_die = ft_atoll(argv[1]);
	data->t_eat = ft_atoll(argv[2]);
	data->t_sleep = ft_atoll(argv[3]);
	data->notepme = -1;
	data->any_dead = false;
	data->philos_done = 0;
	data->start_time = current_mtime();
	if (argc == 6)
		data->notepme = (long)ft_atoll(argv[4]);
	if (pthread_mutex_init(&data->lock_dead, NULL) != 0)
		return (printf(ERROR "pthread_mutex_init for data failed.\n"), false);
	if (pthread_mutex_init(&data->lock_done, NULL) != 0)
		return (pthread_mutex_destroy(&data->lock_dead),
			printf(ERROR "pthread_mutex_init for data failed.\n"), false);
	if (pthread_mutex_init(&data->lock_print, NULL) != 0)
		return (pthread_mutex_destroy(&data->lock_dead),
			pthread_mutex_destroy(&data->lock_done),
			printf(ERROR "pthread_mutex_init for data failed.\n"), false);
	return (true);
}

static bool	ft_init_sphilo(t_data *data)
{
	int	id;

	id = 0;
	data->philo = malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philo == NULL)
		return (pthread_mutex_destroy(&data->lock_dead),
			pthread_mutex_destroy(&data->lock_done), pthread_mutex_destroy(&data
				->lock_print), printf(ERROR "malloc failed.\n"), false);
	while (++id <= data->num_philos)
	{
		data->philo[id - 1].id = id;
		data->philo[id - 1].times_ate = 0;
		data->philo[id - 1].t_of_death = current_mtime() + data->t_die;
		data->philo[id - 1].data = data;
		if (pthread_mutex_init(&data->philo[id - 1].r_fork, NULL) != 0)
			return (ft_exit("mutex_init for philo failed", data->philo, --id,
					false), false);
		if (pthread_mutex_init(&data->philo->lock_eating, NULL) != 0)
			return (pthread_mutex_destroy(&data->philo[id - 1].r_fork),
				ft_exit("mutex_init failed", data->philo, --id, false), false);
		if (id > 1)
			data->philo[id - 1].l_fork = &data->philo[id - 2].r_fork;
	}
	data->philo[0].l_fork = &data->philo[data->num_philos - 1].r_fork;
	return (true);
}

static bool	ft_threads(t_data *data)
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
