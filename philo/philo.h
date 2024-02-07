/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/07 07:03:00 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h> //also for the usleep
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h> //for the gettimeofday
# include <stdint.h>

# define ERROR "\x1B[31mError: \x1B[0m"

typedef enum e_state
{
	takes_fork,
	eats,
	sleeps,
	thinks,
	is_dead,
	finished
}	t_state;

typedef struct s_data	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	long			t_of_death;
	unsigned int	times_ate;
	bool			is_eating;
	t_data			*data;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	lock_eating;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			notepme;
	t_philo			*philo;
	long			start_time;
	bool			any_dead;
	bool			done_eating;
	int				philos_done;
	pthread_mutex_t	lock_dead;
	pthread_mutex_t	lock_done;
	pthread_mutex_t	lock_print;
}	t_data;

long		ft_atol(const char *str);
time_t		current_mtime(void);
void		ft_msleep(long msec);
bool		ft_exit(char *msg, t_philo *philo, int num, bool join);

void		*philo_routine(void *arg);
void		*supervisor_routine(void *arg);
void		ft_print_action(t_philo *philo, t_state action);
bool		reached_the_end(t_data *data);
bool		no_philo_dead(t_philo *philo);
bool		reached_the_end(t_data *data);

#endif //PHILO_H
