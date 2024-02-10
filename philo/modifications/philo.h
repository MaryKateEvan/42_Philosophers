/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/10 20:41:25 by mevangel         ###   ########.fr       */
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
# define INPUT "\x1B[31mInvalid input: \x1B[0m"

typedef enum e_state
{
	takes_fork,
	eats,
	sleeps,
	thinks,
	is_dead
}	t_state;

typedef struct s_data	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	long long		t_of_death;
	long			times_ate;
	bool			is_full;
	t_data			*data;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	// pthread_mutex_t	lock_eating; //! i deleted this and moved it in the data struct
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	long long		t_die;
	long long		t_eat;
	long long		t_sleep;
	long			notepme;
	t_philo			*philo;
	long long		start_time;
	bool			any_dead;
	int				philos_done;
	pthread_mutex_t	lock_time; //for the t_of_death
	pthread_mutex_t	lock_dead; //for the any_dead
	pthread_mutex_t	lock_done; //for the times_ate and philos_done
	pthread_mutex_t	lock_print;
}	t_data;

/* ----------------------------- ROUTINE ----------------------------- */
void		*philo_routine(void *arg);
void		*supervisor_routine(void *arg);

/* ----------------------------- CONTROL ----------------------------- */
bool		reached_the_end(t_data *data);
bool		no_philo_dead(t_philo *philo);
bool		reached_the_end(t_data *data);

/* ------------------------------ UTILS ------------------------------ */
long long	ft_atoll(const char *str);
long long	current_mtime(void);
void		ft_msleep(long long msec);
bool		ft_exit(char *msg, t_data *data, int num, bool join);
void		ft_print_action(t_philo *philo, t_state action);

#endif //PHILO_H
