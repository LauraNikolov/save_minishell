/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:34:43 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 16:24:55 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	len;
	void	*str;

	len = size * count;
	if (len == 0)
	{
		str = malloc(0);
		if (!str)
			return (0);
		return (str);
	}
	else if (len / size != count)
		return (0);
	str = malloc(len);
	if (!str)
		return (0);
	ft_bzero(str, len);
	return (str);
}
