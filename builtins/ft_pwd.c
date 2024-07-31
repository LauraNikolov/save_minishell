/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 23:51:17 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 23:51:18 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(char **cmd, t_envp **envp)
{
	int		i;
	char	*path;

	if (cmd[1])
	{
		if (cmd[1][0] == '-')
		{
			ft_putstr_fd("minishell: pwd: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			return (ft_return_code(ft_strdup("2"), envp));
		}
	}
	path = ft_search_var("PWD", envp);
	i = -1;
	if (path)
	{
		while (path[++i])
			write(1, &path[i], 1);
		write(1, "\n", 1);
	}
	else
		return (ft_return_code(ft_strdup("127"), envp));
	return (ft_return_code(ft_strdup("0"), envp));
}
