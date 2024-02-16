/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:58:51 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/15 06:25:08 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*joined;
	int		i;
	int		j;
	int		size;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = malloc(size);
	if (!joined)
		return (NULL);
	joined[size - 1] = '\0';
	i = -1;
	j = -1;
	while (s1[++i] != '\0')
		joined[i] = s1[i];
	while (s2[++j] != '\0')
		joined[i + j] = s2[j];
	free(s2);
	return (joined);
}
