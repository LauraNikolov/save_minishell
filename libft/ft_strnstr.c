/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 17:19:28 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 17:19:45 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t		i;
	size_t		j;

	i = 0;
	if (!s2 || s2[0] == '\0')
		return ((char *)s1);
	while (i < n && s1[i])
	{
		j = 0;
		if (s1[i] == s2[j])
		{
			while (s1[i + j] == s2[j] && i + j < n)
			{
				j++;
				if (!s2[j])
					return ((char *)s1 + i);
			}
		}
		i++;
	}
	return (NULL);
}

/* int	main(void)
{
	char *s1 = "rtherd";
	char *s2 = "";
	size_t n;

	n = 30;
	printf("%s", ft_strnstr(s1, s2, n));
} */