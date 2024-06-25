/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:25:15 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 17:15:23 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*s1;

	s1 = (char *)s;
	i = 0;
	while (i < n)
	{
		if (s1[i] == (char)c)
			return (&s1[i]);
		i++;
	}
	return (NULL);
}

/*int main (void)
{
	const char s1[]= "ewlfc";
	int n;
	char c;

	n = 1;


	c = 'f';

	printf ("%s\n", (char*)ft_memchr(s1, c, n));
	printf ("%s", (char*)memchr(s1, c, n));
}*/