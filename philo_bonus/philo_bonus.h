/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:54:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/18 02:27:50 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>

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
	t_data			*data;
	char			*time_name;
	sem_t			*lock_time;
	char			*done_name;
	sem_t			*sem_is_done;
}	t_philo;

typedef struct s_data
{
	int			num_philos;
	long long	t_die;
	long long	t_eat;
	long long	t_sleep;
	long		notepme;
	long long	start_time;
	t_philo		*philo;
	pid_t		*pid_array;
	sem_t		*forks;
	sem_t		*sem_dead;
	sem_t		*sem_print;
}	t_data;

/* ------------------------------- CORE ------------------------------- */
void		check_input(int argc, char **argv);

/* ----------------------------- ROUTINES ----------------------------- */
void		*philo_routine(void *arg);
void		*supervisor_routine(void *arg);
void		*count_meals(void *arg);

/* ------------------------------ UTILS ------------------------------- */
long long	current_mtime(void);
void		ft_msleep(long long msec);
void		ft_exit(char *msg, t_data *data, int num, int exit_code);
void		ft_print_action(t_philo *philo, t_state action);
long long	ft_atoll(const char *str);
char		*ft_strjoin(char *s1, char *s2);
char		*small_itoa(int n);

#endif //PHILO_BONUS_H
