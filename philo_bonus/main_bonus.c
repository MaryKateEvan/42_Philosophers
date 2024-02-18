/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/18 02:28:06 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	init_shared_data(int argc, char **argv, t_data *data)
{
	data->num_philos = (int)ft_atoll(argv[0]);
	data->t_die = ft_atoll(argv[1]);
	data->t_eat = ft_atoll(argv[2]);
	data->t_sleep = ft_atoll(argv[3]);
	data->notepme = -1;
	data->start_time = current_mtime();
	if (argc == 6)
		data->notepme = (long)ft_atoll(argv[4]);
	sem_unlink("/forks");
	data->forks = sem_open("/forks", O_CREAT, 0744, data->num_philos);
	if (data->forks == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), false);
	sem_unlink("/dead");
	data->sem_dead = sem_open("/dead", O_CREAT, 0744, 0);
	if (data->sem_dead == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), sem_close(data->forks),
			sem_unlink("/forks"), false);
	sem_unlink("/print");
	data->sem_print = sem_open("/print", O_CREAT, 0744, 1);
	if (data->sem_print == SEM_FAILED)
		return (printf(ERROR "sem_open failed.\n"), sem_close(data->forks),
			sem_unlink("/forks"), sem_close(data->sem_dead),
			sem_unlink("/print"), false);
	return (true);
}

static void	init_philo_data(t_philo *philo, int id, t_data *data)
{
	philo->id = id;
	philo->thread = 0;
	philo->times_ate = 0;
	philo->t_of_death = current_mtime() + data->t_die;
	philo->data = data;
	philo->time_name = ft_strjoin("/dtime_", small_itoa(id));
	if (philo->time_name == NULL)
		ft_exit("malloc failed", data, --id, 1);
	sem_unlink(philo->time_name);
	philo->lock_time = sem_open(philo->time_name, O_CREAT, 0744, 1);
	if (philo->lock_time == SEM_FAILED)
		ft_exit("sem_open failed", data, --id, 2);
	philo->done_name = ft_strjoin("/is_done_", small_itoa(id));
	if (philo->done_name == NULL)
		ft_exit("malloc failed", data, --id, 1);
	sem_unlink(philo->done_name);
	philo->sem_is_done = sem_open(philo->done_name, O_CREAT, 0744, 0);
	if (philo->sem_is_done == SEM_FAILED)
	{
		sem_close(philo->lock_time);
		sem_unlink(philo->time_name);
		free(philo->time_name);
		free(philo->done_name);
		ft_exit("sem_open failed", data, --id, 2);
	}
}

static void	philosopher_process(t_philo *philo, t_data *data)
{
	pthread_t	supervisor;

	if (pthread_create(&philo->thread, NULL, &philo_routine, philo) != 0)
		ft_exit("pthread_create failed", data, data->num_philos, 4);
	if (pthread_create(&supervisor, NULL, &supervisor_routine, philo) != 0)
		ft_exit("pthread_create failed", data, data->num_philos, 4);
	if (pthread_join(supervisor, NULL) != 0)
		ft_exit("pthread_join failed", data, data->num_philos, 5);
	if (pthread_join(philo->thread, NULL) != 0)
		ft_exit("pthread_join failed", data, data->num_philos, 5);
}

static void	ft_simulation(t_data *data)
{
	int			i;
	pthread_t	check_end;

	data->pid_array = malloc((data->num_philos) * (sizeof(pid_t)));
	if (data->pid_array == NULL)
		ft_exit("malloc failed", data, data->num_philos, 1);
	i = -1;
	while (++i < data->num_philos)
	{
		data->pid_array[i] = fork();
		if (data->pid_array[i] == -1)
			ft_exit("fork failed", data, data->num_philos, 3);
		if (data->pid_array[i] == 0)
			philosopher_process(&data->philo[i], data);
	}
	if (data->notepme > 0)
	{
		pthread_create(&check_end, NULL, &count_meals, data);
		pthread_detach(check_end);
	}
	sem_wait(data->sem_dead);
	i = -1;
	while (++i < data->num_philos)
		kill(data->pid_array[i], SIGKILL);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	check_input(argc, ++argv);
	if (!init_shared_data(argc, argv, &data))
		return (2);
	data.philo = malloc((data.num_philos) * (sizeof(t_philo)));
	if (data.philo == NULL)
		ft_exit("malloc failed", &data, 0, 1);
	i = -1;
	while (++i < data.num_philos)
		init_philo_data(&data.philo[i], i + 1, &data);
	ft_simulation(&data);
	ft_exit(NULL, &data, data.num_philos, 0);
	return (EXIT_SUCCESS);
}
