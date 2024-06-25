/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 17:32:00 by melmarti          #+#    #+#             */
/*   Updated: 2024/06/11 13:30:17 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*ft_putnbr(char *s, int n, int counter)
{
	long int	nb;
	int			i;

	i = 0;
	nb = n;
	s[counter] = '\0';
	if (nb < 0)
	{
		nb *= -1;
		s[0] = '-';
		i = 1;
	}
	while (counter-- > i)
	{
		s[counter] = (nb % 10) + 48;
		nb /= 10;
	}
	return (s);
}

static int	ft_count_nb(int n)
{
	int	counter;

	counter = 0;
	if (n <= 0)
		counter++;
	while (n)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}

char	*ft_itoa(int n)
{
	size_t	counter;
	char	*s;

	counter = ft_count_nb(n);
	s = malloc(sizeof(char) * counter + 1);
	if (!s)
		return (NULL);
	return (ft_putnbr(s, n, counter));
}

// int main(void)
// {
//    int n;
//    n = 0;
//    printf ("%s\n", ft_itoa(n));
// }
