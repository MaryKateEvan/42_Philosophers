/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:59:22 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/25 17:59:37 by mevangel         ###   ########.fr       */
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

bool ft_init_sdata(int argc, char **argv, t_data *data)
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
	data->philo = (t_philo *)malloc(data->num_philos * sizeof(t_philo));
	if (data->philo == NULL)
		return(false);
	return (true);
}


bool ft_init_sphilo(t_philo *philo, unsigned int num_philos)
{
	unsigned int	id;

	id = 0;
	while (++id <= num_philos) //i starts as 1
	{
		philo[id - 1].id = id;
		// data->philo[id - 1].thread = 0; //do I actually need that?
		philo[id - 1].started_eat = current_mtime();
		philo[id - 1].times_ate = 0;
		philo[id - 1].is_eating = false;
		if (pthread_mutex_init(&philo[id - 1].r_fork, NULL) != 0) //returns 0 in success
			return (false);
		if (id > 1)
			philo[id - 1].l_fork = &philo[id - 2].r_fork;
	}
	philo[0].l_fork = &philo[num_philos - 1].r_fork;
	// Any mutexes I want to initialize here?
	return (true);
}

void	*routine(void *data)
{
	
}

void ft_error_exit(char *message, t_data *data, int i)
{
	int	i;

	printf(RED "Error: " CLEAR "%s\n", message);
	i = 0;
	while (i < data->num_philos)
	{
		
	}
	free(data->philo);
}

int	main(int argc, char **argv)
{
	t_data			data;
	unsigned int	i;
	// t_philo	*philos_ptr;
	
	if (ft_input_is_valid(argc, ++argv) == false)
		return(printf(RED "Error: " CLEAR "%s\n", "invalid input"), 0); //or i could change it directly to printf
	if (ft_init_sdata(argc, argv, &data) == false)
		return(printf(RED "Error: " CLEAR "%s\n", "malloc failed"), 1); //i don't need to free the data->philo cause if the malloc failed the pointer is null
	if (ft_init_sphilo(data.philo, data.num_philos) == false)
		return(printf(RED "Error: " CLEAR "%s\n", "mlx_init failed"), 1);
	data.start_time = current_mtime();
	i = 0;
	while (i < data.num_philos) //the comparison is with i as 0 but i inside the loop starts as 1
		if (pthread_create(&data.philo[i].thread, NULL, &routine, &data.philo[i++]) != 0) //returns 0 in success
			return(ft_error_exit("pthread_create failed.", &data, ), 1); //! i have to destroy the mutexes and joinn the threads
	
	// create the threads: philos and death/monitor thread
	// join the threads.
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