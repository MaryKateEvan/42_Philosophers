/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/30 20:10:41 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
	- Accepts only numeric characters (0123456789) in the arguments, '+' or '-' not accepted
	- rejects empty arguments
	- rejects 0 as any argument
*/
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

bool ft_init_sdata(int argc, char **argv, t_data *data)
{
	data->num_philos = (int)ft_atoul(argv[0]);
	// printf("num of philos received is: %d\n", data->num_philos);
	if (data->num_philos > 200)
		return (printf(ERROR "philos can not be more than 200.\n"), false);
	data->t_die = ft_atoul(argv[1]);
	data->t_eat = ft_atoul(argv[2]);
	data->t_sleep = ft_atoul(argv[3]);
	data->notepme = 0;
	data->any_dead = false;
	data->done_eating = false;
	data->philos_done = 0;
	data->start_time = current_mtime();
	if (argc == 6)
		data->notepme = (unsigned int)ft_atoul(argv[4]);
	if (pthread_mutex_init(&data->dead, NULL) || pthread_mutex_init(&data->done, NULL)
		|| pthread_mutex_init(&data->print, NULL) || pthread_mutex_init(&data->lock, NULL))
			return (printf(ERROR "pthread_mutex_init_failed.\n"), false);
	return (true);
}

bool	ft_init_sphilo(t_data *data)
{
	int	id;

	id = 0;
	data->philo = malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philo == NULL)
		return (false);
	while (++id <= data->num_philos) //i starts as 1
	{
		data->philo[id - 1].id = id;
		// data->philo[id - 1].thread = 0; //do I actually need that?
		data->philo[id - 1].t_until_death = current_mtime() + data->t_die;
		data->philo[id - 1].times_ate = 0;
		data->philo[id - 1].finished = false;
		data->philo[id - 1].is_eating = false;
		data->philo[id - 1].data = data;
		if (pthread_mutex_init(&data->philo[id - 1].r_fork, NULL)) //returns 0 in success
			return (ft_exit("mutex_init failed", data->philo, --id, false), false);
		if (id > 1)
			data->philo[id - 1].l_fork = &data->philo[id - 2].r_fork;
	}
	data->philo[0].l_fork = &data->philo[data->num_philos - 1].r_fork;
	if (pthread_mutex_init(&data->philo->lock_philo, NULL) != 0)
		return (ft_exit("mutex_init failed", data->philo, --id, false), false);
	return (true);
}

void	*test_routine(void *arg)
{
	t_philo *tmp;

	tmp = (t_philo *) arg;

	printf("hello from philo thread with id %d\n", tmp->id);
	// printf("the saved start time is %lu\n", tmp->data->start_time);
	ft_print_action(tmp, thinks);
	return (tmp);
}

bool	ft_supervisor(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		pthread_mutex_lock(&data->philo[i].lock_philo);
		if (current_mtime() >= data->philo[i].t_until_death &&
			data->philo[i].is_eating == false)
		{
			pthread_mutex_lock(&data->lock);
			data->any_dead = true;
			pthread_mutex_unlock(&data->lock);
			ft_print_action(&data->philo[i], is_dead);
			return (pthread_mutex_unlock(&data->philo[i].lock_philo), true);
		}
		if (data->philo[i].times_ate == data->notepme)
		{
			// data->philo[i].finished = true;
			data->philos_done++;
			if (data->philos_done == data->num_philos)
				data->done_eating = true;
			return (pthread_mutex_unlock(&data->philo[i].lock_philo), true);
		}
		pthread_mutex_unlock(&data->philo[i].lock_philo);
		if (i == (data->num_philos - 1))
			i = -1;
	}
	return (false);
}

bool	ft_threads(t_data *data)
{
	int	i;
	// pthread_t	check_dead;
	bool		end;

	i = -1;
	// lockmutex
	//? i create the philo threads first:
	while (++i < data->num_philos) //the comparison is with i as 0 but i inside the loop starts as 1
		if (pthread_create(&data->philo[i].thread, NULL, &philo_routine, &data->philo[i])) //** i'm passing the data.phio as the 4th argument because the philos "don't communicate with each other"
			return(ft_exit("pthread_create failed", data->philo, data->num_philos, true), false);
	//? and then the death patrol
	/*
	unlockmutex
	Check if dead or eaten enough
		if one dead or all eaten enough
			lock print mutex (and print death msg)
			exit out of the supervisor
	Set all philos to dead
	unlock print mutex
	threadjoin
	*/
	end = false;
	while (data->any_dead == false && end == false)
		end = ft_supervisor(data);
	
	// if (pthread_create(&check_dead, NULL, &ft_death_patrol, data.philo))
	// 	return(ft_exit("pthread_create failed", data.philo, data.num_philos, true), 4);
	// //? and then i join the death thread
	// pthread_join(check_dead, NULL);
	//? and the philo threads: + destroy the mutexes
	if (!ft_exit(NULL, data->philo, data->num_philos, true))
		return(printf(ERROR "pthread_join failed\n"), false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_data		data;


	// t_philo	*philos_ptr;
	
	if (!ft_input_is_valid(argc, ++argv))
		return(printf(ERROR "invalid input\n"), 0); //or i could change it directly to printf
	if (!ft_init_sdata(argc, argv, &data))
		return(printf("Failed to initialize data.\n"), 1); //i don't need to free the data->philo cause if the malloc failed the pointer is null
	if (!ft_init_sphilo(&data)) // it returns non zero only if it failed
		return(printf(ERROR "failed to initialize philos\n"), 2);
	printf("now is: %lu\n", current_mtime());
	if (!ft_threads(&data)) // it returns non zero only if it failed
		return(printf(ERROR "failed to initialize the threads\n"), 3);

	// //join the threads.
	// destroy all the mutexes for the forks
	
	return (EXIT_SUCCESS);
}
