/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:58:46 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/15 13:03:34 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *s, size_t n)
{
	size_t		i;
	char		*desti;
	const char	*str;

	if (!dest && !s)
		return (NULL);
	i = 0;
	desti = dest;
	str = s;
	while (i < n)
	{
		desti[i] = str[i];
		i++;
	}
	return (desti);
}
/*
int	main(void)
{
	const char *src = "zoubyfughbigyuibi";
	char dest[1];

	size_t n;
	n = 20;

	ft_memcpy(dest, src, n);

	printf("%s", dest);
	return (0);
}*/