/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 23:54:58 by lnicolof          #+#    #+#             */
/*   Updated: 2024/08/01 00:05:30 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_limiter(char *s1, char *s2)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (s2[i])
	{
		y = 0;
		while (s2[i] == s1[y] && s1[y] && s2[i])
		{
			i++;
			y++;
			if (y == (int)ft_strlen(s1))
				return (1);
		}
		i++;
	}
	return (0);
}

void	heredoc_parent(pid_t pid, int file)
{
	(void)pid;
	ft_signal(3);
	waitpid(pid, &g_exit_status, 0);
	close(file);
}
