/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 06:40:02 by mevangel         ###   ########.fr       */
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
# include <stdint.h> //? do i actually need that?
# include <semaphore.h>
# include <sys/types.h> //for the pid_t type
# include <signal.h> //for the kill and its macros

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
	bool			is_done;
	t_data			*data;
	char			*sem_name;
	sem_t			*sem_eating;
	char			*sem_done_name;
	sem_t			*sem_is_done;
}	t_philo;

typedef struct s_data
{
	int			num_philos;
	long long	t_die;
	long long	t_eat;
	long long	t_sleep;
	long		notepme;
	t_philo		*philo;
	long long	start_time;
	bool		any_dead;
	int			philos_done;
	sem_t		*forks;
	sem_t		*sem_dead;
	sem_t		*sem_done;
	sem_t		*sem_print;
	pid_t		*pid_array;
}	t_data;

/* ----------------------------- ROUTINE ----------------------------- */
void		*philo_routine(void *arg);
void		*supervisor_routine(void *arg);

/* ----------------------------- CONTROL ----------------------------- */
bool		reached_the_end(t_data *data);
bool		no_philo_dead(t_philo *philo);

/* ------------------------------ UTILS ------------------------------ */
long long	ft_atoll(const char *str);
long long	current_mtime(void);
void		ft_msleep(long long msec);
void		ft_exit(char *msg, t_data *data, int num, int exit_code);
void		ft_print_action(t_philo *philo, t_state action);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_itoa(int n);

#endif //PHILO_H
