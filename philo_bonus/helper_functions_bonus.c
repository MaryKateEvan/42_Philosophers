/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:58:51 by mevangel          #+#    #+#             */
/*   Updated: 2024/02/16 08:49:30 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	ft_atoll(const char *str)
{
	long long	num;

	num = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	return (num);
}

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
		return (NULL);  //! i must check the protection for here
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

char	*ft_small_itoa(int n)
{
	char	*ptr;
	int		len;
	int		tmp;
	int		i;

	tmp = n;
	len = 0;
	while (tmp > 0)
	{
		tmp = tmp / 10;
		len++;
	}
	ptr = (char *)malloc((len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL); //! i need to check better the protection for here
	i = 0;
	while (len > i)
	{
		ptr[len - 1] = (n % 10) + '0';
		len--;
		n = n / 10;
	}
	ptr[len] = '\0';
	return (ptr);
}
