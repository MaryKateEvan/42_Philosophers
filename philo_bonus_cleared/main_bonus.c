/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 12:37:18 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_input(int argc, char **argv)
{
	int	i;

	if (!(argc == 5 || argc == 6))
		ft_exit("expected 5 or 6 arguments.", NULL, 0, 0);
	while (*argv)
	{
		i = 0;
		while ((*argv)[i])
		{
			if ((*argv)[i] < '0' || (*argv)[i] > '9')
				ft_exit("only positive numbers accepted", NULL, 0, 0);
			i++;
		}
		if (!(*argv)[0])
			ft_exit("empty argument(s)", NULL, 0, 0);
		if ((*argv)[0] == '0' && (*argv)[1] == '\0')
			ft_exit("all arguments must be greater than zero", NULL, 0, 0);
		argv++;
	}
}

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
	
	philo->time_name = ft_strjoin("/dtime_", small_itoa(id)); //?should i protect the return from the joins? in case malloc in itoa or strjoin failed
	if (philo->time_name == NULL)
		ft_exit("malloc failed", data, id - 2, 1);
	sem_unlink(philo->time_name);
	philo->lock_time = sem_open(philo->time_name, O_CREAT, 0744, 1);
	if (philo->lock_time == SEM_FAILED)
	{
		free(philo->time_name);
		ft_exit("sem_open failed", data, id - 2, 2);
	}
	
	philo->done_name = ft_strjoin("/is_done_", small_itoa(id));
	if (philo->done_name == NULL)
		ft_exit("malloc failed", data, id - 2, 1);
	sem_unlink(philo->done_name);
	philo->sem_is_done = sem_open(philo->done_name, O_CREAT, 0744, 0);
	if (philo->sem_is_done == SEM_FAILED)
	{
		sem_close(philo->lock_time);
		sem_unlink(philo->time_name);
		free(philo->time_name);
		free(philo->done_name);
		ft_exit("sem_open failed", data, id - 2, 2);
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
	int			i;
	pthread_t	check_end;

	data->pid_array = malloc((data->num_philos) * sizeof(pid_t));
	if (data->pid_array == NULL)
		ft_exit("malloc failed", data, data->num_philos, 1);
	i = -1;
	while (++i < data->num_philos)
	{
		data->pid_array[i] = fork();
		if (data->pid_array[i] == -1) //to protect the case that fork fails
			ft_exit("fork failed", data, data->num_philos, 3);
		if (data->pid_array[i] == 0) //for child process = philosopher process
			philosopher_process(&data->philo[i], data);
	}
	pthread_create(&check_end, NULL, &count_meals, data);
	pthread_detach(check_end);
	// pthread_join(check_end, NULL);
	sem_wait(data->sem_dead);
	i = -1;
	while (++i < data->num_philos)
		kill(data->pid_array[i], SIGKILL);
	free(data->pid_array);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	check_input(argc, ++argv);
	if (!init_shared_data(argc, argv, &data))
		return (1);
	data.philo = malloc((data.num_philos) * (sizeof(t_philo)));
	if (data.philo == NULL)
		ft_exit("malloc failed", &data, 0, 1);
	i = -1;
	while (++i < data.num_philos)
		init_philo_data(&(data.philo[i]), i + 1, &data);
	// ft_init_sphilo(&data);
	ft_simulation(&data);
	ft_exit(NULL, &data, data.num_philos, 0);
	return (EXIT_SUCCESS);
}

// // the previous function:
// static void	ft_init_sphilo(t_data *data)
// {
// 	int	id;


// 	id = 0;
// 	while (++id <= data->num_philos)
// 	{
// 		data->philo[id - 1].id = id;
// 		data->philo[id - 1].thread = 0;
// 		data->philo[id - 1].times_ate = 0;
// 		data->philo[id - 1].t_of_death = current_mtime() + data->t_die;
// 		data->philo[id - 1].data = data;
		
// 		data->philo[id - 1].time_name = ft_strjoin("/dtime_", small_itoa(id)); //?should i protect the return from the joins? in case malloc in itoa or strjoin failed
// 		if (data->philo[id - 1].time_name == NULL)
// 			ft_exit("malloc failed", data, --id, 1);
// 		sem_unlink(data->philo[id - 1].time_name);
// 		data->philo[id - 1].lock_time = sem_open(data->philo[id - 1].time_name,
// 			O_CREAT, 0744, 1);
// 		if (data->philo[id - 1].lock_time == SEM_FAILED)
// 		{
// 			free(data->philo[id - 1].time_name);
// 			ft_exit("sem_open failed", data, --id, 2);
// 		}
		
// 		data->philo[id - 1].done_name = ft_strjoin("/is_done_", small_itoa(id));
// 		if (data->philo[id - 1].done_name == NULL)
// 			ft_exit("malloc failed", data, --id, 1);
// 		sem_unlink(data->philo[id - 1].done_name);
// 		data->philo[id - 1].sem_is_done = sem_open(data->philo[id - 1].done_name,
// 			O_CREAT, 0744, 0);
// 		if (data->philo[id - 1].sem_is_done == SEM_FAILED)
// 		{
// 			sem_close(data->philo[id - 1].lock_time);
// 			sem_unlink(data->philo[id - 1].time_name);
// 			free(data->philo[id - 1].time_name);
// 			free(data->philo[id - 1].done_name);
// 			ft_exit("sem_open failed", data, --id, 2);
// 		}
// 	}
// }