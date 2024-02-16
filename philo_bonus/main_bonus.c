/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 07:59:32 by mevangel         ###   ########.fr       */
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
	sem_unlink("/dead");
	sem_unlink("/done");
	sem_unlink("/print");
	data->sem_dead = sem_open("/dead", O_CREAT, 0744, 0);
	if (data->sem_dead == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), false);
	data->sem_done = sem_open("/done", O_CREAT, 0744, 1);
	if (data->sem_done == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), sem_close(data->sem_dead),
			sem_unlink("/dead"), false);
	data->sem_print = sem_open("/print", O_CREAT, 0744, 1);
	if (data->sem_print == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), sem_close(data->sem_dead),
			sem_unlink("/dead"), sem_close(data->sem_done), sem_unlink("/done"),
				false);
	data->philo = malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philo == NULL)
		ft_exit("malloc failed", data, 0, 1);
	return (true);
}

static void	ft_init_sphilo(t_data *data)
{
	int	id;

	id = 0;
	while (++id <= data->num_philos)
	{
		data->philo[id - 1].id = id;
		data->philo[id - 1].thread = 0;
		data->philo[id - 1].times_ate = 0;
		data->philo[id - 1].is_done = false;
		data->philo[id - 1].t_of_death = current_mtime() + data->t_die;
		data->philo[id - 1].data = data;
		data->philo[id - 1].sem_name = ft_strjoin("/eating_", ft_itoa(id));
		sem_unlink(data->philo[id - 1].sem_name);
		data->philo[id - 1].sem_eating = sem_open(data->philo[id - 1].sem_name, O_CREAT, 0644, 1);
		if (data->philo[id - 1].sem_eating == SEM_FAILED)
			ft_exit("sem_open failed", data, --id, 2);
		data->philo[id - 1].sem_done_name = ft_strjoin("/is_done_", ft_itoa(id));
		sem_unlink(data->philo[id - 1].sem_done_name);
		data->philo[id - 1].sem_is_done = sem_open(data->philo[id - 1].sem_done_name, O_CREAT, 0644, 0);
		if (data->philo[id - 1].sem_is_done == SEM_FAILED)
			ft_exit("sem_open failed", data, --id, 2);
	}
}

static void	philosopher_process(t_philo *philo, t_data *data)
{
	pthread_t	supervisor;

	pthread_create(&philo->thread, NULL, &philo_routine, philo);//! protect it later
	if (pthread_create(&supervisor, NULL, &supervisor_routine, philo) != 0)
		ft_exit("pthread_create failed", data, data->num_philos, 3);
	// philo_routine(philo);
	if (pthread_join(supervisor, NULL) != 0)
		ft_exit("pthread_join failed", data, data->num_philos, 4);
	pthread_join(philo->thread, NULL);
}

void	*count_meals(void *arg)
{
	t_data	*data;
	int 	i;

	data = (t_data *)arg;
	i = -1;
	while (++i < data->num_philos)
	{
		sem_wait(data->philo[i].sem_is_done);
	}
	sem_wait(data->sem_print);
	sem_post(data->sem_dead);
	return (NULL);
}

static void	ft_simulation(t_data *data)
{
	int		i;
	pthread_t	check_end;

	sem_unlink("/forks");
	data->forks = sem_open("/forks", O_CREAT, 0744, data->num_philos);
	if (data->forks == SEM_FAILED)
		ft_exit("sem_open failed", data, data->num_philos, 2);
	data->pid_array = malloc(sizeof(pid_t) * data->num_philos);
	if (data->pid_array == NULL)
		ft_exit("malloc failed", data, data->num_philos, 1);
	i = -1;
	while (++i < data->num_philos)
	{
		data->pid_array[i] = fork();
		if (data->pid_array[i] == -1) //to protect the case that fork fails
		{
			sem_close(data->forks);
			sem_unlink("/forks");
			ft_exit("fork failed", data, data->num_philos, 2);
		}
		if (data->pid_array[i] == 0) //for child process = philosopher process
			philosopher_process(&data->philo[i], data);
	}
	pthread_create(&check_end, NULL, &count_meals, data);
	pthread_detach(check_end);
	sem_wait(data->sem_dead);
	i = -1;
	while (++i < data->num_philos)
		kill(data->pid_array[i], SIGKILL);
	sem_close(data->forks);
	sem_unlink("/forks");
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!ft_input_is_valid(argc, ++argv))
		return (0);
	if (!ft_init_sdata(argc, argv, &data))
		return (1);
	ft_init_sphilo(&data);
	ft_simulation(&data);
	ft_exit(NULL, &data, data.num_philos, 0);
	return (EXIT_SUCCESS);
}
