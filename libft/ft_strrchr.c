/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:02:11 by melmarti          #+#    #+#             */
/*   Updated: 2024/05/13 17:29:22 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strrchr(const char *s, int c)
{
	int	j;

	j = 0;
	while (s[j])
		j++;
	while (j >= 0)
	{
		if (s[j] == (unsigned char)c)
			return (j);
		j--;
	}
	return (j);
}
