/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:04:18 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/10 18:00:27 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	unsigned const char	*s;
	int					i;

	d = (unsigned char *)dst;
	s = (unsigned const char *)src;
	i = 0;
	if (d < s && n != 0)
	{
		while ((size_t)i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	if (s < d && n != 0)
	{
		i = n - 1;
		while (i >= 0)
		{
			d[i] = s[i];
			i--;
		}
	}
	return (d);
}
/* int	main(void)
{
	char sResult[] = {67, 68, 67, 68, 69, 0, 45};
	printf("%s", ((char *)ft_memmove(sResult + 1, sResult, 2)));
}  */