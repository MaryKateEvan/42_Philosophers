/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/15 04:38:34 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	data->sem_dead = sem_open("/dead", O_CREAT, 0644, 1);
	// if (data->sem_dead == SEM_FAILED)
	// 	return (printf(ERROR "sem_open failed.\n"), false);
	data->sem_done = sem_open("/done", O_CREAT, 0644, 1);
	// if (data->sem_done == SEM_FAILED)
	// 	return (printf(ERROR "sem_open failed.\n"), sem_close(data->sem_dead),
	// 		sem_unlink("/dead"), false);
	data->sem_print = sem_open("/print", O_CREAT, 0644, 1);
	// if (data->sem_print == SEM_FAILED)
	// 	return (printf(ERROR "sem_open failed.\n"), sem_close(data->sem_dead),
	// 		sem_unlink("/dead"), sem_close(data->sem_done), sem_unlink("/done"),
	// 			false);
	data->philo = malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philo == NULL)
		ft_exit("malloc failed", data, 0, 1);
	return (true);
}

static void	ft_init_sphilo(t_philo *philo, t_data *data)
{
	int	id;

	id = 0;
	while (++id <= data->num_philos)
	{
		philo[id - 1].id = id;
		philo[id - 1].times_ate = 0;
		philo[id - 1].is_done = false;
		philo[id - 1].t_of_death = current_mtime() + data->t_die;
		philo[id - 1].sem_name = ft_strjoin("/eating_", ft_itoa(id));
		philo[id - 1].sem_eating = sem_open(philo[id - 1].sem_name, O_CREAT, 0644, 1);
		if (philo[id - 1].sem_eating == SEM_FAILED)
			ft_exit("sem_open failed", data, --id, 2);
	}
}

// static bool	philosopher_process(t_philo *philo, t_data *data, sem_t *forks)
// {
// 	pthread_t	supervisor;

// 	if (pthread_create(&supervisor, NULL, &supervisor_routine, data) != 0)
// 		return (ft_exit("pthread_create failed", data, data->num_philos,
// 				3), false);
// 	philo_routine(philo, data, forks);
// 	if (pthread_join(supervisor, NULL) != 0)
// 		return (ft_exit("pthread_join failed", data, data->num_philos, 4), false);
// 	return (true);
// }

static void	ft_simulation(t_philo *philo, t_data *data)
{
	int		i;
	pid_t	pid;
	sem_t	*sem_forks;
	pthread_t	supervisor;

	sem_forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
	if (sem_forks == SEM_FAILED)
		ft_exit("sem_open failed", data, data->num_philos, 2);
	i = -1;
	while (++i < data->num_philos)
	{
		pid = fork();
		if (pid == -1) //to protect the case that fork fails
		{
			sem_close(sem_forks);
			sem_unlink("/forks");
			ft_exit("fork failed", data, data->num_philos, 2);
		}
		if (pid == 0) //for child process = philosopher process
			philo_routine(philo + i, data, sem_forks);
	}
	pthread_create(&supervisor, NULL, &supervisor_routine, data);
	i = -1;
	while (++i < data->num_philos)
		waitpid(-1, NULL, 0);
	pthread_join(supervisor, NULL);
	sem_close(sem_forks);
	sem_unlink("/forks");
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!ft_input_is_valid(argc, ++argv))
		return (0);
	if (!ft_init_sdata(argc, argv, &data))
		return (1);
	ft_init_sphilo(data.philo, &data);
	ft_simulation(data.philo, &data);
	return (EXIT_SUCCESS);
}
