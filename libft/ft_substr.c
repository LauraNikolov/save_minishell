/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 17:20:27 by melmarti          #+#    #+#             */
/*   Updated: 2023/11/17 17:20:28 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	if ((size_t)start >= ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(&s[start]))
	{
		len = ft_strlen(&s[start]);
	}
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (0);
	while (i < len && start + i < ft_strlen(s))
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
/* int main (void)
{
	char s[] = "tripouille";
	unsigned int start;
	size_t len;

	start = 100;
	len = 1;
	printf ("%s", ft_substr(s, start, len));
} */