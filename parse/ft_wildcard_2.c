/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:12:41 by melmarti          #+#    #+#             */
/*   Updated: 2024/07/29 14:13:08 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_match(char *cmd, char *dir)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (cmd[i] && cmd[i] != '*')
		i++;
	while (cmd[k + i] && cmd[k + i] == '*')
		k++;
	if ((!cmd || !cmd[0]) && dir)
		return (0);
	if (!ft_strncmp(cmd, dir, i))
	{
		if (cmd[i + k - 1] == '*' && !cmd[i + k])
			return (1);
		if (!cmd[i + k] && !dir[i])
			return (1);
		while (cmd[i + k] && dir[j] && cmd[i + k] != dir[j])
			j++;
		j += ft_is_char(&dir[j + 1], dir[j]);
		return (ft_match(&cmd[i + k], &dir[j]));
	}
	return (0);
}
