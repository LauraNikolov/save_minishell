/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolof <lnicolof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:37:50 by lnicolof          #+#    #+#             */
/*   Updated: 2024/08/01 00:12:32 by lnicolof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

void	error_exec_str(char *str, char *cmd, int exit_code)
{
	ft_putstr_fd("minishell :", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	write(2, "\n", 1);
	exit(exit_code);
}

void	ft_parse_error(t_cmd *cmd)
{
	struct stat	buf;

	if (stat(cmd->cmd[0], &buf) != -1)
	{
		if (((S_ISDIR(buf.st_mode)) && (!ft_strncmp("./", cmd->cmd[0], 2)))
			|| (cmd->cmd[0][ft_strlen(cmd->cmd[0]) - 1] == '/'))
			error_exec_str("Is a directory", cmd->cmd[0], 126);
		if (!(buf.st_mode & S_IXUSR) || !(buf.st_mode & S_IRUSR)
			|| !S_ISLNK(buf.st_mode) || !S_ISDIR(buf.st_mode))
			error_exec_str("Permission denied", cmd->cmd[0], 126);
	}
	if (ft_strncmp("./", cmd->cmd[0], 2) == 0 || ft_strncmp("/", cmd->cmd[0],
			1) == 0)
		error_exec_str("No such file or directory", cmd->cmd[0], 127);
	else
		error_exec_str("command not found", cmd->cmd[0], 127);
}

void	redir_error(char *str)
{
	struct stat	buf;

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
