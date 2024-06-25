/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:33:24 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/13 12:53:10 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	u;

	i = 0;
	u = 0;
	if (!src || !dest)
		return (0);
	u = ft_strlen(src);
	if (n != 0)
	{
		while (i < n - 1 && src[i])
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (u);
}

/* int main (void)
{
	char dest[10];
	const char *src = "idgr";
	size_t n;
	n = 4;

	ft_strlcpy(dest, src, n);
	printf ("%s\n", dest);

	printf ("%zu\n\n\n", ft_strlcpy(dest,src, n));

   ft_strlcpy(dest, src, n);

	printf ("%s\n", dest);

	printf ("%zu\n\n\n", ft_strlcpy(dest,src, n));
} */
