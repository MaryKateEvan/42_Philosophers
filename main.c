/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/28 19:13:35 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Checks for: 
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

void	ft_init_sdata(int argc, char **argv, t_data *data)
{
	data->num_philos = ft_atoui(argv[0]);
	data->time_to_die = ft_atoui(argv[1]);
	data->time_eating = ft_atoui(argv[2]);
	data->time_sleeping = ft_atoui(argv[3]);
	data->max_meals = 0;
	if (argc == 6)
		data->max_meals = ft_atoui(argv[4]);
	data->any_dead = false;
	// Should I also save the multiplication max_meals * num_of_philos ?

}

// bool ft_init_sdata(int argc, char **argv, t_data *data)
// {
// 	data->num_philos = ft_atoui(argv[0]);
// 	data->time_to_die = ft_atoui(argv[1]);
// 	data->time_eating = ft_atoui(argv[2]);
// 	data->time_sleeping = ft_atoui(argv[3]);
// 	data->max_meals = 0;
// 	if (argc == 6)
// 		data->max_meals = ft_atoui(argv[4]);
// 	data->any_dead = false;
// 	// Should I also save the multiplication max_meals * num_of_philos ?

// 	return (true);
// }

bool	ft_init_sphilo(t_data *data)
{
	unsigned int	id;

	id = 0;
	data->philo = (t_philo *)malloc(data->num_philos * sizeof(t_philo));
	if (data->philo == NULL)
		return (false);
	while (++id <= data->num_philos) //i starts as 1
	{
		data->philo[id - 1].id = id;
		// data->philo[id - 1].thread = 0; //do I actually need that?
		data->philo[id - 1].started_eat = current_mtime();
		data->philo[id - 1].times_ate = 0;
		data->philo[id - 1].is_eating = false;
		if (pthread_mutex_init(&data->philo[id - 1].r_fork, NULL)) //returns 0 in success
			return (ft_error("mutex_init failed", data->philo, --id, false), false);
		if (id > 1)
			data->philo[id - 1].l_fork = &data->philo[id - 2].r_fork;
	}
	data->philo[0].l_fork = &data->philo[data->num_philos - 1].r_fork;
	// Any mutexes I want to initialize here?
	return (true);
}

void	*test_routine(void *arg)
{
	t_philo *tmp;

	tmp = (t_philo *)arg;

	printf("hello from thread %p\n", &tmp);
	return (tmp);
}

void	*ft_death_patrol(void *arg)
{
	
}

int	main(int argc, char **argv)
{
	t_data			data;
	unsigned int	i;
	pthread_t		death;
	// t_philo	*philos_ptr;
	
	if (!ft_input_is_valid(argc, ++argv))
		return(printf(ERROR "invalid input\n"), 0); //or i could change it directly to printf
	ft_init_sdata(argc, argv, &data);
	// if (!ft_init_sdata(argc, argv, &data))
	// 	return(printf(ERROR "malloc failed\n"), 1); //i don't need to free the data->philo cause if the malloc failed the pointer is null
	if (!ft_init_sphilo(&data)) // it returns non zero only if it failed
		return(printf(ERROR "failed to initialize philos\n"), 2);
	data.start_time = current_mtime();
	i = 0;
	while (i++ < data.num_philos) //the comparison is with i as 0 but i inside the loop starts as 1
		if (pthread_create(&data.philo[i - 1].thread, NULL, &test_routine, &data.philo[i - 1])) //returns 0 in success
			return(ft_error("pthread_create failed", data.philo, data.num_philos, true), 3);
	// fo the death watch thread
	if (pthread_create(&death, NULL, &ft_death_patrol, data.philo))
		return(ft_error("pthread_create failed", data.philo, data.num_philos, true), 3);
	pthread_join(death, NULL);
	// join everything and destroy the mutexes:
	if (!ft_error(NULL, data.philo, data.num_philos, true))
		return(printf(ERROR "pthread_join failed\n"), 4);
	// //join the threads.
	// destroy all the mutexes for the forks
	
	return (EXIT_SUCCESS);
}

/* the big function i had initially and i splitted */

// bool ft_init_sdata(int argc, char **argv, t_data *data)
// {
// 	data->num_philos = ft_atoui(argv[0]);
// 	data->time_to_die = ft_atoui(argv[1]);
// 	data->time_eating = ft_atoui(argv[2]);
// 	data->time_sleeping = ft_atoui(argv[3]);
// 	data->max_meals = 0;
// 	if (argc == 6)
// 		data->max_meals = ft_atoui(argv[4]);
// 	data->any_dead = false;
// 	// Should I also save the multiplication max_meals * num_of_philos ?
// 	data->philo = (t_philo *)malloc(data->num_philos * sizeof(t_philo));
// 	if (data->philo == NULL)
// 		return(false);
// 	// while (++id <= data->num_philos) //i starts as 1
// 	// {
// 	// 	data->philo[id - 1].id = id;
// 	// 	// data->philo[id - 1].thread = 0; //do I actually need that?
// 	// 	data->philo[id - 1].started_eat = current_mtime();
// 	// 	data->philo[id - 1].times_ate = 0;
// 	// 	data->philo[id - 1].is_eating = false;
// 	// 	if (pthread_mutex_init(&data->philo[id - 1].r_fork, NULL) != 0) //returns 0 in success
// 	// 		return (false);
// 	// 	if (id > 1)
// 	// 		data->philo[id - 1].l_fork = &data->philo[id - 2].r_fork;
// 	// }
// 	// data->philo[0].l_fork = &data->philo[data->num_philos - 1].r_fork;
// 	// Any mutexes I want to initialize here?
// 	return (true);
// }