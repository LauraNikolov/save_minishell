/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:16:20 by melmarti          #+#    #+#             */
/*   Updated: 2024/06/07 15:16:59 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strdup_array(char **cmd)
{
	int		i;
	char	**cpy;

	i = 0;
	if (!cmd || !*cmd)
		return (NULL);
	while (cmd[i])
		i++;
	cpy = malloc((i + 1) * sizeof(char *));
	if (!cpy)
		return (NULL);
	i = -1;
	while (cmd[++i])
	{
		cpy[i] = ft_strdup(cmd[i]);
		if (!cpy[i])
		{
			while (i > 0)
				free(cpy[--i]);
			free(cpy);
			return (NULL);
		}
	}
	cpy[i] = NULL;
	return (cpy);
}
