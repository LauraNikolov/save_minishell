/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:33:36 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/16 16:52:58 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_src;
	size_t	len_dst;
	size_t	cpy_len;

	i = 0;
	if ((!dst || !src) && size == 0)
		return (0);
	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	if (size <= len_dst)
	{
		return (len_src + size);
	}
	cpy_len = size - len_dst - 1;
	while (i < cpy_len && src[i] != '\0')
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (len_src + len_dst);
}

/*int	main(void)
{
	char	*dst;
	size_t	n;
	int		n1;
	char	*dst1;

	dst = "coucreyrd";
	char src[] = "zoubizoubizoubi";
	n = 6;
	printf("%zu", ft_strlcat(dst, src, n));
	printf("\n%s\n", dst);
	dst1 = "coucreyrd";
	char src1[] = "zoubizoubizoubi";
	n1 = 6;
	printf("%d", strlcat(dst1, src1, n));
	printf("\n%s", dst1);
}*/