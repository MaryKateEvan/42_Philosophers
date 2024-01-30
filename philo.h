/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/01/30 05:08:35 by mevangel         ###   ########.fr       */
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

// # define RED "\033[0;31m"
// # define CLEAR "\033[0m"
# define RESET_COLOR "\x1b[0m"
# define BLACK "\x1b[30m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define WHITE "\x1b[37m"

// Bright versions
# define BR_BLACK "\x1b[90m"
# define BR_RED "\x1b[91m"
# define BR_GREEN "\x1b[92m"
# define BR_YELLOW "\x1b[93m"
# define BR_BLUE "\x1b[94m"
# define BR_MAGENTA "\x1b[95m"
# define BR_CYAN "\x1b[96m"
# define BR_WHITE "\x1b[97m"

// # define RED "\x1B[31m"
# define CLEAR "\x1B[0m"

// Define macros for colored output
# define ERROR "\x1B[31mError: \x1B[0m"

typedef struct s_philo t_philo;

typedef enum e_state
{
	takes_fork,
	eats,
	sleeps,
	thinks,
	is_dead,
	finished
}	t_state;

typedef struct s_data
{
	int				num_philos;
	unsigned long	t_die;
	unsigned long	t_eat;
	unsigned long	t_sleep;
	unsigned int	notepme; //numbers of times each philosopher must eat
	t_philo			*philo;
	unsigned long	start_time; //! long or int?
	bool			any_dead;
	pthread_mutex_t	dead; //to protect the modification of this struct's data
	int				philos_done;
	pthread_mutex_t	done;
	pthread_mutex_t	print; //to protect the modification of this struct's data
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	unsigned long	t_until_death;
	// unsigned long	started_eat;
	unsigned int	times_ate;
	bool			finished; //yes if the philo ate notepme
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	// pthread_mutex_t	philo_lock; //i haven't used it yet
	t_data			*data;
}	t_philo;


unsigned long	ft_atoul(const char *str);
unsigned long	current_mtime(void);
void			ft_msleep(unsigned long msec);
bool			ft_exit(char *msg, t_philo *philo, int num, bool join);

void			*philo_routine(void *arg);
void			ft_actions(t_philo *philo, bool *stop);
void			ft_print_action(t_philo *philo, t_state action);

#endif //PHILO_H

// ./philo num_philos time_to_die time_eating time_sleeping times_to_eat[optional];

// when "times_ate" becomes equal == "notepme" then i set the "finished_meals" for the philo true
// and i increase the "philos_done". When "philos_done" == "num_philos" the program is over
