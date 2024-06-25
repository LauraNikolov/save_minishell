/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:25:22 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 17:14:00 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *p, int value, size_t count)
{
	size_t			i;
	unsigned char	*dest;

	dest = (unsigned char *)p;
	i = 0;
	while (i < count)
	{
		dest[i] = (unsigned char)value;
		i++;
	}
	return (p);
}
