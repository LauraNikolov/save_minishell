/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:37:50 by lnicolof          #+#    #+#             */
/*   Updated: 2024/07/31 12:46:24 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>


void redir_error(char *str)
{
	struct stat buf;

	if (stat(str, &buf) != -1)
	{
		if (S_ISDIR(buf.st_mode))
		{
			
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(str, 2);
			ft_putstr_fd(": Is a directory\n", 2);
		}
		else if (access(str, F_OK) != -1 && access(str, R_OK | W_OK) == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(str, 2);
            ft_putstr_fd(": Permission denied\n", 2);
        }
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

void	exit_error(char *str, t_save_struct *t_struct)
{
	ft_putstr_fd(str, 2);
	ft_free_envp_lst(&t_struct->envp, NULL);
	ft_all_free(t_struct, 0, NULL);
	exit(134);
}
