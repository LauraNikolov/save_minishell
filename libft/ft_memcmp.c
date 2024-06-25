/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:25:59 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 16:20:07 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *string1, const void *string2, size_t n)

{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)string1;
	s2 = (unsigned char *)string2;
	i = 0;
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

/*int	main(void)
{
	const char s1[] = "Helloword";
	const char s2[] = "Hellounivers";
	size_t n;

	char s[] = {-128, 0, 127, 0};
	char sCpy[] = {-128, 0, 127, 0};
	n = 4;

	printf("%d\n", ft_memcmp(s, sCpy, n));
	printf("%d", memcmp(s, sCpy, n));
}*/
