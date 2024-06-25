/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jointab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:30:17 by melmarti          #+#    #+#             */
/*   Updated: 2024/06/07 13:30:57 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_cpy(char **dest, char **src, int *index)
{
	int	j;

	j = 0;
	while (src[j])
	{
		dest[*index] = ft_strdup(src[j]);
		if (!dest[*index])
		{
			while (--(*index) >= 0)
				free(dest[*index]);
			return (NULL);
		}
		(*index)++;
		j++;
	}
	return (dest);
}

char	**ft_jointab(char **tab1, char **tab2)
{
	char	**str;
	int		i;

	if (!tab1 || !tab2)
		return (NULL);
	str = malloc(sizeof(char *) * (ft_count_tab(tab1) + ft_count_tab(tab2)
				+ 1));
	if (!str)
		return (NULL);
	i = 0;
	if (!ft_cpy(str, tab1, &i) || !ft_cpy(str, tab2, &i))
	{
		while (--i >= 0)
			free(str[i]);
		free(str);
		return (NULL);
	}
	str[i] = NULL;
	free(tab1);
	free(tab2);
	return (str);
}
