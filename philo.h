/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/25 03:16:52 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h> //for the gettimeofday

# define RED "\033[0;31m"
# define CLEAR "\033[0m"

typedef struct s_philos t_philos;

typedef struct s_data
{
	// pthread_t	*philos; //array of threads, each is a philo
	unsigned int	num_philos;
	unsigned int	time_to_die;
	unsigned int	time_eating;
	unsigned int	time_sleeping;
	unsigned int	max_meals;
	t_philo			*philo;
	unsigned int	start_time;
	bool			any_dead; //zero or one according if philo is dead
	// bool			program_done;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	unsigned int	id;
	unsigned int	started_eat;
	unsigned int	times_ate;
	bool			is_eating; //i have not used it yet
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;

	//time: time_to_die;
	pthread_mutex_t	philo_lock; //not pointer 
	// don't forget to link tot he main data struct
}	t_philo;

unsigned int	ft_atoui(const char *str);
unsigned int	current_mtime(void);

#endif //PHILO_H

// ./philo num_philos time_to_die time_eating time_sleeping times_to_eat[optional];