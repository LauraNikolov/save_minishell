/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:05 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 16:44:23 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_access(t_cmd *node, char **abs_path)
{
	if (*abs_path)
	{
		if (access(*abs_path, F_OK) == 0)
		{
			node->path = *abs_path;
			return (1);
		}
		ft_safe_free(abs_path);
	}
	return (0);
}

void	ft_get_path(t_cmd *node, t_save_struct *t_struct)
{
	char	*path;
	char	*absolute_path;
	char	**bin;
	int		i;

	if (!node->cmd)
		return ;
	path = getenv("PATH");
	if (!path)
		exit_error("Getenv failed\n", t_struct);
	bin = ft_split(path, ":");
	i = -1;
	while (bin[++i])
	{
		absolute_path = ft_strjoin_path(bin[i], node->cmd[0]);
		if (ft_access(node, &absolute_path) == 1)
			break ;
	}
	if (!ft_access(node, &absolute_path))
	{
		ft_safe_free(&node->path);
		node->path = ft_strdup(node->cmd[0]);
	}
	ft_free_tab(bin);
}
